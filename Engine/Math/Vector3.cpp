#include "Vector3.h"
#include <cassert>
#include <cmath>

const Vector3 Vector3::Zero = Vector3(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Right = Vector3(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up = Vector3(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Forward = Vector3(0.0f, 0.0f, 1.0f);

Vector3& Vector3::operator+=(const Vector3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

Vector3& Vector3::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	z *= scale;
	return *this;
}

Vector3& Vector3::operator/=(float scale)
{
	assert(scale != 0.0f);
	x /= scale;
	y /= scale;
	z /= scale;
	return *this;
}

bool Vector3::operator==(const Vector3& other)
{
	return x == other.x && y == other.y && z == other.z;
}

bool Vector3::operator!=(const Vector3& other)
{
	return x != other.x || y != other.y || z != other.z;
}

Vector3 Vector3::operator-() const
{
	return Vector3(-x, -y, -z);
}

std::wstring Vector3::ToString()
{
	wchar_t buffer[256];
	swprintf_s(buffer, 256, L"(%f,%f,%f)", x, y, z);
	return buffer;
}

float Vector3::Length()
{
	return sqrtf(x * x + y * y + z * z);
}

Vector3 Vector3::Normalized()
{
	float length = Length();
	return Vector3(x / length, y / length, z / length);
}

bool Vector3::Equals(const Vector3& other)
{
	return *this == other;
}

Vector3 operator+(const Vector3& left, const Vector3& right)
{
	return Vector3(left.x + right.x, left.y + right.y, left.z + right.z);
}

Vector3 operator-(const Vector3& left, const Vector3& right)
{
	return Vector3(left.x - right.x, left.y - right.y, left.z - right.z);
}

Vector3 operator*(const Vector3& vector, float scale)
{
	return Vector3(vector.x * scale, vector.y * scale, vector.z * scale);
}

Vector3 operator*(float scale, const Vector3& vector)
{
	return vector * scale;
}

Vector3 operator/(const Vector3& vector, float scale)
{
	assert(scale != 0.0f);
	return Vector3(vector.x / scale, vector.y / scale, vector.z / scale);
}

float Dot(const Vector3& left, const Vector3& right)
{
	return left.x * right.x + left.y * right.y + left.z * right.z;
}

Vector3 Cross(const Vector3& left, const Vector3& right)
{
	return Vector3
	(
		left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x
	);
}

Vector3 Lerp(const Vector3& from, const Vector3& to, float t)
{
	// t °¡µÎ±â.
	//t = t < 0.0f ? 0.0f : t < 1.0f ? 1.0f : t;
	if (t < 0.0f)
	{
		t = 0.0f;
	}
	if (t > 1.0f)
	{
		t = 1.0f;
	}

	return (1.0f - t) * from + t * to;
}