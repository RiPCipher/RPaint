// Default Shader
// NOTE: no #version here on purpose. The loader prepends "#version 330 core"
// and the stage #define (VERTEX_SHADER / FRAGMENT_SHADER) ahead of this file

// ------------- Vertex ---------------

#ifdef VERTEX_SHADER

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec3 aNormPos;
layout (location = 3) in vec4 aTangent;

out vec2 texPos;
out vec3 worldPos;
out mat3 TBN;
out vec4 fragPosLightSpace;

uniform mat4 lightSpaceMatrix;
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main() {
	vec4 world = Model * vec4(vertPos, 1.0);
	fragPosLightSpace = lightSpaceMatrix * world;
	gl_Position = Projection * View * world;

	worldPos = world.xyz;
	texPos   = aTexPos;

	mat3 normalMat = transpose(inverse(mat3(Model)));
	vec3 N = normalize(normalMat * aNormPos);
	vec3 T = normalize(normalMat * aTangent.xyz);
	T = normalize(T - N * dot(N, T));
	vec3 B = cross(N, T) * aTangent.w;

	TBN = mat3(T, B, N);
}

#endif

// ------------- Fragment ---------------

#ifdef FRAGMENT_SHADER

in vec2 texPos;
in vec3 worldPos;
in mat3 TBN;
in vec4 fragPosLightSpace;


out vec4 fragOut;

uniform sampler2D shadowMap;

uniform sampler2D aTexture;
uniform sampler2D aNormalTexture;
uniform sampler2D aORMTexture; // packed: R = Occlusion, G = Roughness, B = Metallic

uniform vec4 baseColorFactor;
uniform float roughnessFactor;
uniform float metallicFactor;
uniform float occlusionFactor;

uniform vec3 camPos;
uniform float ambientStrength = 0.1;

const int MAX_POINT_LIGHTS = 32;
const int MAX_SPOT_LIGHTS  = 32;


const float PI = 3.14159265359;

struct DirectionalLight {
	vec4 direction; // w = unused
	vec4 color;     // w = intensity
};

struct OmniLight {
	vec4 position; // w = range
	vec4 color;    // w = intensity
};

struct SpotLight {
	vec4 position;  // w = range
	vec4 direction; // w = cosInner
	vec4 color;     // w = intensity
};

layout(std140) uniform LightBlock {
	DirectionalLight sun;
	ivec4            counts;
	OmniLight        points[MAX_POINT_LIGHTS];
	SpotLight        spots[MAX_SPOT_LIGHTS];
};

float ShadowCalculation(vec4 fragPosLightSpace, vec3 N, vec3 L) {
    vec3 proj = fragPosLightSpace.xyz / fragPosLightSpace.w;
    proj = proj * 0.5 + 0.5;
    if (proj.z > 1.0) return 0.0;

    float currentDepth = proj.z;
    float bias = max(0.005 * (1.0 - dot(N, L)), 0.0005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x)
        for (int y = -1; y <= 1; ++y) {
            float pcf = texture(shadowMap, proj.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcf ? 1.0 : 0.0;
        }
    return shadow / 9.0;
}

float distribution(vec3 N, vec3 H, float roughness);
float geometrySchlick(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);
vec3 cookTorrance(vec3 N, vec3 V, vec3 L, vec3 radiance, vec3 albedo, float roughness, float metallic, vec3 F0);

void main() {
	// Material
	vec4  baseColor = texture(aTexture, texPos) * baseColorFactor;
	vec3  albedo    = baseColor.rgb;
	vec3  normal    = texture(aNormalTexture, texPos).rgb * 2.0 - 1.0;

	vec3  orm       = texture(aORMTexture, texPos).rgb;
	float ao        = mix(1.0, orm.r, occlusionFactor);
	float roughness = clamp(orm.g * roughnessFactor, 0.04, 1.0);
	float metallic  = clamp(orm.b * metallicFactor,  0.0,  1.0);

	vec3 N = normalize(TBN * normal);
	vec3 V = normalize(camPos - worldPos);

	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	vec3 Lo = vec3(0.0);

	//Directional light
	{
		vec3 L        = normalize(-sun.direction.xyz);
		vec3 radiance = sun.color.rgb * sun.color.w;
		float shadow  = ShadowCalculation(fragPosLightSpace, N, L);
		Lo += (1.0 - shadow) * cookTorrance(N, V, L, radiance, albedo, roughness, metallic, F0);
	}

	// Point lights
	for (int i = 0; i < counts.x; ++i) {
		vec3  toLight = points[i].position.xyz - worldPos;
		float dist    = length(toLight);
		float range   = points[i].position.w;

		vec3  L       = toLight / dist;
		float atten   = max(1.0 - dist / range, 0.0);
		vec3  radiance = points[i].color.rgb * points[i].color.w * atten;

		Lo += cookTorrance(N, V, L, radiance, albedo, roughness, metallic, F0);
	}

	// Ambient + output
	//vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 skyColor    = vec3(0.38, 0.45, 0.55);
	vec3 groundColor = vec3(0.20, 0.17, 0.14);
	float tN = N.y * 0.5 + 0.5;
	vec3 irradiance = mix(groundColor, skyColor, tN);
	vec3 R = reflect(-V, N);
	float tR = R.y * 0.5 + 0.5;
	vec3 envSpecular = mix(groundColor, skyColor, tR);
	vec3 F  = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
	vec3 kD = (vec3(1.0) - F) * (1.0 - metallic);
	vec3 ambient = (kD * irradiance * albedo + envSpecular * F) * ao;

	vec3 color   = ambient + Lo;

	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	fragOut = vec4(color, 1.0); //fragOut = vec4(N*0.5+0.5, 1)  //fragOut = vec4(vec3(roughness), 1)  //fragOut = vec4(vec3(metallic), 1)
}

//BRDF
float distribution(vec3 N, vec3 H, float roughness) {
	float a  = roughness * roughness;
	float a2 = a * a;
	float NdotH  = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;
	return a2 / denom;
}

float geometrySchlick(float NdotV, float roughness) {
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;
	return NdotV / (NdotV * (1.0 - k) + k);
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	return geometrySchlick(NdotV, roughness) * geometrySchlick(NdotL, roughness);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 cookTorrance(vec3 N, vec3 V, vec3 L, vec3 radiance,
                  vec3 albedo, float roughness, float metallic, vec3 F0) {
	vec3 H = normalize(V + L);

	float NDF = distribution(N, H, roughness);
	float G   = geometrySmith(N, V, L, roughness);
	vec3  F   = fresnelSchlick(max(dot(H, V), 0.0), F0);

	vec3  numerator   = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
	vec3  specular    = numerator / denominator;

	vec3 kS = F;
	vec3 kD = (vec3(1.0) - kS) * (1.0 - metallic);

	float NdotL = max(dot(N, L), 0.0);
	return (kD * albedo / PI + specular) * radiance * NdotL;
}

#endif
