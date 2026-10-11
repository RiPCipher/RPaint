#include "assimp_loader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/config.h>

#include <string>
#include <iostream>

namespace {
	constexpr int STRIDE = 12;

	constexpr unsigned int POST_PROCESS =
		  aiProcess_Triangulate
		| aiProcess_JoinIdenticalVertices
		| aiProcess_GenSmoothNormals      // only fills in normals when the file has none
		| aiProcess_CalcTangentSpace
		| aiProcess_SortByPType
		| aiProcess_ValidateDataStructure;

	void configure(Assimp::Importer& importer) {
		// Drop point/line primitives so every mesh we see is pure triangles
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);
	}

	void appendMesh(const aiMesh* mesh, const aiNode* node,
	                const aiMatrix4x4& global, const aiMatrix3x3& basis, const aiMatrix3x3& normalMat,
	                bool mirrored, MeshData& out) {
		if (!(mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE)) return;

		const unsigned int base = static_cast<unsigned int>(out.vertices.size() / STRIDE);
		const aiVector3D* uvs = mesh->mTextureCoords[0];
		const bool hasTangents = mesh->mTangents && mesh->mBitangents;

		out.vertices.reserve(out.vertices.size() + mesh->mNumVertices * STRIDE);
		for (unsigned int v = 0; v < mesh->mNumVertices; ++v) {
			aiVector3D p = global * mesh->mVertices[v];
			aiVector3D n = mesh->mNormals ? (normalMat * mesh->mNormals[v]).NormalizeSafe() : aiVector3D(0.0f);

			out.vertices.push_back(p.x);
			out.vertices.push_back(p.y);
			out.vertices.push_back(p.z);

			out.vertices.push_back(uvs ? uvs[v].x : 0.0f);
			out.vertices.push_back(uvs ? uvs[v].y : 0.0f);

			out.vertices.push_back(n.x);
			out.vertices.push_back(n.y);
			out.vertices.push_back(n.z);

			if (hasTangents) {
				aiVector3D t = (basis * mesh->mTangents[v]).NormalizeSafe();
				aiVector3D b = basis * mesh->mBitangents[v];
				float w = ((n ^ t) * b < 0.0f) ? -1.0f : 1.0f;

				out.vertices.push_back(t.x);
				out.vertices.push_back(t.y);
				out.vertices.push_back(t.z);
				out.vertices.push_back(w);
			} else {
				// No UVs -> no tangent space
				out.vertices.push_back(1.0f);
				out.vertices.push_back(0.0f);
				out.vertices.push_back(0.0f);
				out.vertices.push_back(1.0f);
			}
		}

		SubMesh sub;
		sub.name = mesh->mName.length > 0 ? mesh->mName.C_Str() : node->mName.C_Str();
		sub.indexOffset = static_cast<unsigned int>(out.indices.size());
		sub.materialIndex = mesh->mMaterialIndex;

		out.indices.reserve(out.indices.size() + mesh->mNumFaces * 3);
		for (unsigned int f = 0; f < mesh->mNumFaces; ++f) {
			const aiFace& face = mesh->mFaces[f];
			if (face.mNumIndices != 3) continue;

			// A mirrored transform flips winding, swap to keep front faces CCW
			out.indices.push_back(base + face.mIndices[0]);
			out.indices.push_back(base + face.mIndices[mirrored ? 2 : 1]);
			out.indices.push_back(base + face.mIndices[mirrored ? 1 : 2]);
		}

		sub.indexCount = static_cast<unsigned int>(out.indices.size()) - sub.indexOffset;
		out.submeshes.push_back(std::move(sub));
	}

	void appendNode(const aiScene* scene, const aiNode* node, const aiMatrix4x4& parent, MeshData& out) {
		aiMatrix4x4 global = parent * node->mTransformation;
		aiMatrix3x3 basis(global);
		aiMatrix3x3 normalMat = basis;
		normalMat.Inverse().Transpose();
		bool mirrored = basis.Determinant() < 0.0f;

		for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
			appendMesh(scene->mMeshes[node->mMeshes[i]], node, global, basis, normalMat, mirrored, out);
		}
		for (unsigned int i = 0; i < node->mNumChildren; ++i) {
			appendNode(scene, node->mChildren[i], global, out);
		}
	}

	// Flattens the node hierarchy into one interleaved buffer, baking node transforms
	MeshData convert(const Assimp::Importer& importer, const aiScene* scene, const std::string& source) {
		MeshData out;

		if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode) {
			std::cerr << "[AssimpLoader] failed to load " << source << ": " << importer.GetErrorString() << std::endl;
			return out;
		}

		appendNode(scene, scene->mRootNode, aiMatrix4x4(), out);

		if (out.indices.empty()) {
			std::cerr << "[AssimpLoader] " << source << " contains no triangle geometry" << std::endl;
		}
		return out;
	}
}

bool AssimpLoader::canLoad(std::string_view ext) const {
	if (ext.empty()) return false;

	std::string dotted = ext.front() == '.' ? std::string(ext) : "." + std::string(ext);
	return Assimp::Importer().IsExtensionSupported(dotted);
}

MeshData AssimpLoader::loadFile(const std::string& path) {
	Assimp::Importer importer;
	configure(importer);

	const aiScene* scene = importer.ReadFile(path, POST_PROCESS);
	return convert(importer, scene, path);
}

MeshData AssimpLoader::load(std::span<const uint8_t> bytes, std::string_view ext) {
	Assimp::Importer importer;
	configure(importer);

	std::string hint(ext.starts_with('.') ? ext.substr(1) : ext);
	const aiScene* scene = importer.ReadFileFromMemory(bytes.data(), bytes.size(), POST_PROCESS, hint.c_str());
	return convert(importer, scene, "<memory:" + hint + ">");
}
