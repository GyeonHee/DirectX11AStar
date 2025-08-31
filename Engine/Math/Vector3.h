#pragma once

#include <string>
#include "Core.h"

// 3���� ���� Ŭ����.
class Engine_API Vector3
{
public:
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	Vector3(float value)
		: x(value), y(value), z(value)
	{
	}

	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		: x(x), y(y), z(z)
	{
	}

	static unsigned int Stride() { return sizeof(Vector3); }

	// ������ �����ε�.
	Vector3& operator+=(const Vector3& other);
	Vector3& operator-=(const Vector3& other);
	Vector3& operator*=(float scale);
	Vector3& operator/=(float scale);

	bool operator==(const Vector3& other);
	bool operator!=(const Vector3& other);

	Vector3 operator-() const;

	Engine_API friend Vector3 operator+(const Vector3& left, const Vector3& right);
	Engine_API friend Vector3 operator-(const Vector3& left, const Vector3& right);

	Engine_API friend Vector3 operator*(const Vector3& vector, float scale);
	Engine_API friend Vector3 operator*(float scale, const Vector3& vector);

	Engine_API friend Vector3 operator/(const Vector3& vector, float scale);

	// (x, y) ���ڿ� ���� �Լ�.
	std::wstring ToString();

	// ���� ���ϱ�.
	float Length();

	// ����(Dot Product/Inner Product).
	Engine_API friend float Dot(const Vector3& left, const Vector3& right);

	// ����(Cross Product/Outer Product).
	Engine_API friend Vector3 Cross(const Vector3& left, const Vector3& right);

	Vector3 Normalized();
	bool Equals(const Vector3& other);

	// ���� ����(Lerp - Linear Interpolation).
	// (1-t)*from + t*to;
	Engine_API friend Vector3 Lerp(const Vector3& from, const Vector3& to, float t);

	// �⺻ ��.
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Right;
	static const Vector3 Up;
	static const Vector3 Forward;
};