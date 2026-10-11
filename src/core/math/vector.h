#pragma once

#include <cmath>

////////////////////////////////////////////////////////////////

template <typename V2>
class Vector2 {
public:

	V2 x, y;

	Vector2(void) :
		x(0), y(0) {}

	Vector2(const V2& _t) :
		x(_t), y(_t) {}

	Vector2(const V2& _x, const V2& _y) :
		x(_x), y(_y) {}

	V2 length(void) const {
		return std::sqrt(x * x + y * y);
	}

	V2 dot(const Vector2<V2>& _var) const {
		return x * _var.x + y * _var.y;
	}

	void normalize(V2 _amp = 1.0) {
		V2 amp = length();
		x /= amp;
		y /= amp;

		if (_amp != 1.0f) *this *= _amp;
	}

	V2 normalized(V2 _amp = 1.0) const {
		Vector2 vec{ x, y };
		vec.normalize(_amp);
		return vec;
	}

	// Operators

	Vector2<V2> operator - (void) const {
		return Vector2<V2>(-x, -y);
	}

	Vector2<V2> operator + (const Vector2<V2>& _var) const {
		return Vector2<V2>(x + _var.x, y + _var.y);
	}
	Vector2<V2> operator - (const Vector2<V2>& _var) const {
		return Vector2<V2>(x - _var.x, y - _var.y);
	}

	Vector2<V2> operator * (const V2& scalar) const {
		return Vector2<V2>(x * scalar, y * scalar);
	}
	Vector2<V2> operator / (const V2& scalar) const {
		return Vector2<V2>(x / scalar, y / scalar);
	}

	Vector2<V2>& operator += (const Vector2<V2>& _var) {
		x += _var.x; y += _var.y;
		return *this;
	}
	Vector2<V2>& operator -= (const Vector2<V2>& _var) {
		x -= _var.x; y -= _var.y;
		return *this;
	}
	Vector2<V2>& operator *= (const V2& scalar) {
		x *= scalar; y *= scalar;
		return *this;
	}
	Vector2<V2>& operator /= (const V2& scalar) {
		x /= scalar; y /= scalar;
		return *this;
	}
};

////////////////////////////////////////////////////////////////

template <typename V3>
class Vector3 {
public:

	V3 x, y, z;

	Vector3(void) :
		x(0), y(0), z(0) {}

	Vector3(const V3& _t) :
		x(_t), y(_t), z(_t) {}

	Vector3(const V3& _x, const V3& _y, const V3& _z) :
		x(_x), y(_y), z(_z) {}

	V3 length(void)const {
		return std::sqrt(x * x + y * y + z * z);
	}

	V3 dot(const Vector3<V3>& _var) const {
		return x * _var.x + y * _var.y + z * _var.z;
	}

	Vector3 cross(const Vector3<V3>& _var) const {
		return Vector3(
			y * _var.z - z * _var.y,
			z * _var.x - x * _var.z,
			x * _var.y - y * _var.x
		);
	}

	void normalize(V3 _amp = 1.0) {
		V3 amp = length();
		x /= amp;
		y /= amp;
		z /= amp;

		if (_amp != 1.0f) *this *= _amp;
	}

	Vector3 normalized(V3 _amp = 1.0) const {
		Vector3 vec{ x, y, z };
		vec.normalize(_amp);
		return vec;
	}

	static inline Vector3<V3> up() {
		return Vector3<V3>(0, 1, 0);
	}

	// Operators

	Vector3<V3> operator - (void) const {
		return Vector3<V3>(-x, -y, -z);
	}

	Vector3<V3> operator + (const Vector3<V3>& _var) const {
		return Vector3<V3>(x + _var.x, y + _var.y, z + _var.z);
	}
	Vector3<V3> operator - (const Vector3<V3>& _var) const {
		return Vector3<V3>(x - _var.x, y - _var.y, z - _var.z);
	}

	Vector3<V3> operator * (const V3& scalar) const {
		return Vector3<V3>(x * scalar, y * scalar, z * scalar);
	}
	Vector3<V3> operator / (const V3& scalar) const {
		return Vector3<V3>(x / scalar, y / scalar, z / scalar);
	}

	Vector3<V3>& operator += (const Vector3<V3>& _var) {
		x += _var.x; y += _var.y; z += _var.z;
		return *this;
	}
	Vector3<V3>& operator -= (const Vector3<V3>& _var) {
		x -= _var.x; y -= _var.y; z -= _var.z;
		return *this;
	}
	Vector3<V3>& operator *= (const V3& scalar) {
		x *= scalar; y *= scalar; z *= scalar;
		return *this;
	}
	Vector3<V3>& operator /= (const V3& scalar) {
		x /= scalar; y /= scalar; z /= scalar;
		return *this;
	}

};

////////////////////////////////////////////////////////////////

template <typename V4>
class Vector4 {
public:

	V4 x, y, z, w;

	Vector4(void) :
		x(0), y(0), z(0), w(0) {}

	Vector4(const V4& _t) :
		x(_t), y(_t), z(_t), w(_t) {}

	Vector4(const V4& _x, const V4& _y, const V4& _z, const V4& _w) :
		x(_x), y(_y), z(_z), w(_w) {}

	V4 length(void) const {
		return std::sqrt(x * x + y * y + z * z + w * w);
	}

	V4 dot(const Vector4<V4>& _var) const {
		return x * _var.x + y * _var.y + z * _var.z + w * _var.w;
	}

	// Operators

	Vector4<V4> operator - (void) const {
		return Vector4<V4>(-x, -y, -z, -w);
	}

	Vector4<V4> operator + (const Vector4<V4>& _var) const {
		return Vector4<V4>(x + _var.x, y + _var.y, z + _var.z, w + _var.w);
	}
	Vector4<V4> operator - (const Vector4<V4>& _var) const {
		return Vector4<V4>(x - _var.x, y - _var.y, z - _var.z, w - _var.w);
	}

	Vector4<V4> operator * (const V4& scalar) const {
		return Vector4<V4>(x * scalar, y * scalar, z * scalar, w * scalar);
	}
	Vector4<V4> operator / (const V4& scalar) const {
		return Vector4<V4>(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	Vector4<V4>& operator += (const Vector4<V4>& _var) {
		x += _var.x; y += _var.y; z += _var.z; w += _var.w;
		return *this;
	}
	Vector4<V4>& operator -= (const Vector4<V4>& _var) {
		x -= _var.x; y -= _var.y; z -= _var.z; w -= _var.w;
		return *this;
	}
	Vector4<V4>& operator *= (const V4& scalar) {
		x *= scalar; y *= scalar; z *= scalar; w *= scalar;
		return *this;
	}
	Vector4<V4>& operator /= (const V4& scalar) {
		x /= scalar; y /= scalar; z /= scalar; w /= scalar;
		return *this;
	}

}; /////////// End Class ////////////////////

template <typename T>
Vector3<T> operator * (const T& scalar, const Vector3<T>& v) {
	return v * scalar;
}