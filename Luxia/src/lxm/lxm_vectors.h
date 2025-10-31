#pragma once

#include <cmath>

// This is the math library for Luxia, LXM. A parody of GLM

namespace lxm {

	struct Vec2 // A struct holding two floats (x, y)
	{
		float x, y;
		Vec2() : x(0), y(0) {}
		Vec2(float x) : x(x), y(x) {}
		Vec2(float x, float y) : x(x), y(y) {}

		Vec2& operator+=(const Vec2& other) {
			x += other.x;
			y += other.y;
			return *this;
		}
		Vec2& operator-=(const Vec2& other) {
			x -= other.x;
			y -= other.y;
			return *this;
		}
		Vec2& operator*=(float scalar) {
			x *= scalar;
			y *= scalar;
			return *this;
		}
		Vec2& operator*=(const Vec2& other) {
			x *= other.x;
			y *= other.y;
			return *this;
		}
		Vec2& operator/=(float scalar) {
			x /= scalar;
			y /= scalar;
			return *this;
		}
		Vec2& operator/=(const Vec2& other) {
			x /= other.x;
			y /= other.y;
			return *this;
		}
	};

	inline Vec2 operator+(Vec2 left, const Vec2& right) { return Vec2(left.x + right.x, left.y + right.y); }
	inline Vec2 operator-(Vec2 left, const Vec2& right) { return Vec2(left.x - right.x, left.y - right.y); }
	inline Vec2 operator/(float scalar, const Vec2& right) { return Vec2(scalar / right.x, scalar / right.y); }
	inline Vec2 operator/(const Vec2 left, float scalar) { return Vec2(left.x / scalar, left.y / scalar); }
	inline Vec2 operator*(float scalar, const Vec2& right) { return Vec2(scalar * right.x, scalar * right.y); }
	inline Vec2 operator*(const Vec2 left, float scalar) { return Vec2(scalar * left.x, scalar * left.y); }


	struct Vec3 // A struct holding three floats (x, y, z)
	{
		float x, y, z;
		Vec3() : x(0), y(0), z(0) {}
		Vec3(float x) : x(x), y(x), z(x) {}
		Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

		Vec3& operator+=(const Vec3& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return *this;
		}
		Vec3& operator-=(const Vec3& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return *this;
		}
		Vec3& operator*=(float scalar) {
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}
		Vec3& operator*=(const Vec3& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}
		Vec3& operator/=(float scalar) {
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}
		Vec3& operator/=(const Vec3& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return *this;
		}
	};
	inline Vec3 operator+(Vec3 left, const Vec3& right) { return Vec3(left.x + right.x, left.y + right.y, left.z + right.z); }
	inline Vec3 operator-(Vec3 left, const Vec3& right) { return Vec3(left.x - right.x, left.y - right.y, left.z - right.z); }
	inline Vec3 operator/(float scalar, const Vec3& right) { return Vec3(scalar / right.x, scalar / right.y, scalar / right.z); }
	inline Vec3 operator/(const Vec3 left, float scalar) { return Vec3(left.x / scalar, left.y / scalar, left.z / scalar); }
	inline Vec3 operator*(float scalar, const Vec3& right) { return Vec3(scalar * right.x, scalar * right.y, scalar * right.z); }
	inline Vec3 operator*(const Vec3 left, float scalar) { return Vec3(scalar * left.x, scalar * left.y, scalar * left.z); }


	inline float length(const Vec2& v) { // Returns length of the vector
		return std::sqrt(v.x * v.x + v.y * v.y);
	}
	inline float length(const Vec3& v) { // Returns length of the vector
		return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	}

	inline Vec2 normalize(const Vec2& v) { // Returns the vector scaled to a length of one unit
		float len = length(v);
		return Vec2(v.x / len, v.y / len);
	}
	inline Vec3 normalize(const Vec3& v) { // Returns the vector scaled to a length of one unit
		float len = length(v);
		return Vec3(v.x / len, v.y / len, v.z / len);
	}

	inline float dot(const Vec2& a, const Vec2& b) { // Returns the dot product, 1 if similar, -1 if opposite
		return a.x * b.x + a.y * b.y;
	}
	inline float dot(const Vec3& a, const Vec3& b) { // Returns the dot product, 1 if similar, -1 if opposite
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}
}