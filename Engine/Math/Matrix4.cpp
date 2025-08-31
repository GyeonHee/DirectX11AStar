#include "Matrix4.h"
#include <cmath>

#include "DirectXMath.h"
using namespace DirectX;

Matrix4 Matrix4::Identity = Matrix4();

Matrix4::Matrix4()
{
	// 단위 행렬 만들기.
	memset(elements, 0, sizeof(float) * 16);
	m00 = m11 = m22 = m33 = 1.0f;
}

Matrix4::Matrix4(const Matrix4& other)
{
	memcpy(elements, other.elements, sizeof(float) * 16);
}

Matrix4 Matrix4::Translation(const Vector3& position)
{
	return Translation(position.x, position.y, position.z);
}

Matrix4 Matrix4::Translation(float x, float y, float z)
{
	Matrix4 m;

	// 행렬.
	m.m00 = 1.0f;	m.m01 = 0.0f;	m.m02 = 0.0f;	m.m03 = 0.0f;
	m.m10 = 0.0f;	m.m11 = 1.0f;	m.m12 = 0.0f;	m.m13 = 0.0f;
	m.m20 = 0.0f;	m.m21 = 0.0f;	m.m22 = 1.0f;	m.m23 = 0.0f;
	m.m30 = x;		m.m31 = y;		m.m32 = z;		m.m33 = 1.0f;

	return m;
}

Matrix4 Matrix4::Rotation(const Vector3& rotation)
{
	return Rotation(rotation.x, rotation.y, rotation.z);
}

Matrix4 Matrix4::Rotation(float x, float y, float z)
{
	return RotationX(x) * RotationY(y) * RotationZ(z);
}

Matrix4 Matrix4::RotationX(float angle)
{
	// 반환을 위한 행렬 변수 선언(단위 행렬).
	Matrix4 m;

	// 행렬 계산을 위한 삼각함수 값 저장.
	float cosAngle = std::cos(angle * degreeToRadian);
	float sinAngle = std::sin(angle * degreeToRadian);

	// 행렬.
	m.m00 = 1.0f;	m.m01 = 0.0f;		m.m02 = 0.0f;		m.m03 = 0.0f;
	m.m10 = 0.0f;	m.m11 = cosAngle;	m.m12 = sinAngle;	m.m13 = 0.0f;
	m.m20 = 0.0f;	m.m21 = -sinAngle;	m.m22 = cosAngle;	m.m23 = 0.0f;
	m.m30 = 0.0f;	m.m31 = 0.0f;		m.m32 = 0.0f;		m.m33 = 1.0f;

	return m;
}

Matrix4 Matrix4::RotationY(float angle)
{
	// 반환을 위한 행렬 변수 선언(단위 행렬).
	Matrix4 m;

	// 행렬 계산을 위한 삼각함수 값 저장.
	float cosAngle = std::cos(angle * degreeToRadian);
	float sinAngle = std::sin(angle * degreeToRadian);

	// 행렬.
	m.m00 = cosAngle;	m.m01 = 0.0f;		m.m02 = -sinAngle;	m.m03 = 0.0f;
	m.m10 = 0.0f;		m.m11 = 1.0f;		m.m12 = 0.0f;		m.m13 = 0.0f;
	m.m20 = sinAngle;	m.m21 = 0.0f;		m.m22 = cosAngle;	m.m23 = 0.0f;
	m.m30 = 0.0f;		m.m31 = 0.0f;		m.m32 = 0.0f;		m.m33 = 1.0f;

	return m;
}

Matrix4 Matrix4::RotationZ(float angle)
{
	// 반환을 위한 행렬 변수 선언(단위 행렬).
	Matrix4 m;

	// 행렬 계산을 위한 삼각함수 값 저장.
	float cosAngle = std::cos(angle * degreeToRadian);
	float sinAngle = std::sin(angle * degreeToRadian);

	// 행렬.
	m.m00 = cosAngle;	m.m01 = sinAngle;	m.m02 = 0.0f;	m.m03 = 0.0f;
	m.m10 = -sinAngle;	m.m11 = cosAngle;	m.m12 = 0.0f;	m.m13 = 0.0f;
	m.m20 = 0.0f;		m.m21 = 0.0f;		m.m22 = 1.0f;	m.m23 = 0.0f;
	m.m30 = 0.0f;		m.m31 = 0.0f;		m.m32 = 0.0f;	m.m33 = 1.0f;

	return m;
}

Matrix4 Matrix4::Scale(const Vector3& scale)
{
	return Scale(scale.x, scale.y, scale.z);
}

Matrix4 Matrix4::Scale(float x, float y, float z)
{
	Matrix4 m;

	// 행렬.
	m.m00 = x;		m.m01 = 0.0f;	m.m02 = 0.0f;	m.m03 = 0.0f;
	m.m10 = 0.0f;	m.m11 = y;		m.m12 = 0.0f;	m.m13 = 0.0f;
	m.m20 = 0.0f;	m.m21 = 0.0f;	m.m22 = z;		m.m23 = 0.0f;
	m.m30 = 0.0f;	m.m31 = 0.0f;	m.m32 = 0.0f;	m.m33 = 1.0f;

	return m;
}

Matrix4 Matrix4::Scale(float scale)
{
	return Scale(scale, scale, scale);
}

Matrix4 Matrix4::Transpose(const Matrix4& target)
{
	Matrix4 m = target;

	std::swap(m.m01, m.m10);
	std::swap(m.m02, m.m20);
	std::swap(m.m03, m.m30);

	std::swap(m.m12, m.m21);
	std::swap(m.m13, m.m31);
	std::swap(m.m23, m.m32);

	return m;
}

Matrix4 Matrix4::LookAt(const Vector3& position, const Vector3& target, const Vector3& up)
{
	Vector3 forward = (target - position).Normalized();
	Vector3 right = Cross(up, forward).Normalized();
	Vector3 upDir = Cross(forward, right).Normalized();

	float x = Dot(right, position);
	float y = Dot(upDir, position);
	float z = Dot(forward, position);

	//XMMatrixLookAtLH

	Matrix4 m;
	m.m00 = right.x;	m.m01 = right.y;	m.m02 = right.z;	m.m03 = -x;
	m.m10 = upDir.x;	m.m11 = upDir.y;	m.m12 = upDir.z;	m.m13 = -y;
	m.m20 = forward.x;	m.m21 = forward.y;	m.m22 = forward.z;	m.m23 = -z;
	m.m30 = 0.0f;		m.m31 = 0.0f;		m.m32 = 0.0f;		m.m33 = 1.0f;

	//m.m00 = right.x;	m.m01 = right.y;	m.m02 = right.z;	m.m03 = 0.0f;
	//m.m10 = upDir.x;	m.m11 = upDir.y;	m.m12 = upDir.z;	m.m13 = 0.0f;
	//m.m20 = forward.x;	m.m21 = forward.y;	m.m22 = forward.z;	m.m23 = 0.0f;
	//m.m30 = -x;			m.m31 = -y;			m.m32 = -z;			m.m33 = 1.0f;

	return m;
}

Matrix4 Matrix4::Perspective(
	float fieldOfView,
	float width,
	float height,
	float nearDistance,
	float farDistance)
{
	//XMMatrixPerspectiveFovLH

	// 종횡비 (화면의 가로/세로 비율).
	float aspect = width / height;

	// 시야각 변환.
	float fov = fieldOfView / 2.0f * degreeToRadian;

	// 초점거리 (d).
	float h = 1.0f / std::tanf(fov);

	float w = h / aspect;

	float a = (nearDistance + farDistance) / (farDistance - nearDistance);

	float b = (-2.0f * nearDistance * farDistance) / (farDistance - nearDistance);

	// 투영 행렬 조립.
	Matrix4 m;

	// 행렬.
	m.m00 = w;		m.m01 = 0.0f;	m.m02 = 0.0f;	m.m03 = 0.0f;
	m.m10 = 0.0f;	m.m11 = h;		m.m12 = 0.0f;	m.m13 = 0.0f;
	m.m20 = 0.0f;	m.m21 = 0.0f;	m.m22 = a;		m.m23 = 1.0f;
	m.m30 = 0.0f;	m.m31 = 0.0f;	m.m32 = b;		m.m33 = 0.0f;

	return m;
}

Matrix4 Matrix4::Orthographic(float width, float height, float nearDistance, float farDistance)
{
	float range = 1.0f / (farDistance - nearDistance);

	Matrix4 m;
	m.m00 = 2.0f / width;	m.m01 = 0.0f;			m.m02 = 0.0f;					m.m03 = 0.0f;
	m.m10 = 0.0f;			m.m11 = 2.0f / height;	m.m12 = 0.0f;					m.m13 = 0.0f;
	m.m20 = 0.0f;			m.m21 = 0.0f;			m.m22 = range;					m.m23 = 0.0f;
	m.m30 = 0.0f;			m.m31 = 0.0f;			m.m32 = -range * nearDistance;	m.m33 = 1.0f;

	return m;
}

Matrix4& Matrix4::operator=(const Matrix4& other)
{
	memcpy(elements, other.elements, sizeof(float) * 16);
	return *this;
}

Matrix4 Matrix4::operator*(const Matrix4& other)
{
	// 반환을 위한 변수 선언.
	Matrix4 m;
	m.m00 = m00 * other.m00 + m01 * other.m10 + m02 * other.m20 + m03 * other.m30;
	m.m01 = m00 * other.m01 + m01 * other.m11 + m02 * other.m21 + m03 * other.m31;
	m.m02 = m00 * other.m02 + m01 * other.m12 + m02 * other.m22 + m03 * other.m32;
	m.m03 = m00 * other.m03 + m01 * other.m13 + m02 * other.m23 + m03 * other.m33;

	m.m10 = m10 * other.m00 + m11 * other.m10 + m12 * other.m20 + m13 * other.m30;
	m.m11 = m10 * other.m01 + m11 * other.m11 + m12 * other.m21 + m13 * other.m31;
	m.m12 = m10 * other.m02 + m11 * other.m12 + m12 * other.m22 + m13 * other.m32;
	m.m13 = m10 * other.m03 + m11 * other.m13 + m12 * other.m23 + m13 * other.m33;

	m.m20 = m20 * other.m00 + m21 * other.m10 + m22 * other.m20 + m23 * other.m30;
	m.m21 = m20 * other.m01 + m21 * other.m11 + m22 * other.m21 + m23 * other.m31;
	m.m22 = m20 * other.m02 + m21 * other.m12 + m22 * other.m22 + m23 * other.m32;
	m.m23 = m20 * other.m03 + m21 * other.m13 + m22 * other.m23 + m23 * other.m33;

	m.m30 = m30 * other.m00 + m31 * other.m10 + m32 * other.m20 + m33 * other.m30;
	m.m31 = m30 * other.m01 + m31 * other.m11 + m32 * other.m21 + m33 * other.m31;
	m.m32 = m30 * other.m02 + m31 * other.m12 + m32 * other.m22 + m33 * other.m32;
	m.m33 = m30 * other.m03 + m31 * other.m13 + m32 * other.m23 + m33 * other.m33;

	return m;
}

Matrix4 Matrix4::operator*=(const Matrix4& other)
{
	*this = *this * other;
	return *this;
}

Vector3 operator*(const Matrix4& matrix, const Vector3& vector)
{
	return vector * matrix;
}

Vector3 operator*(const Vector3& vector, const Matrix4& matrix)
{
	Vector3 result;

	result.x = vector.x * matrix.m00 + vector.y * matrix.m10 + vector.z * matrix.m20;
	result.y = vector.x * matrix.m01 + vector.y * matrix.m11 + vector.z * matrix.m21;
	result.z = vector.x * matrix.m02 + vector.y * matrix.m12 + vector.z * matrix.m22;

	return result;
}