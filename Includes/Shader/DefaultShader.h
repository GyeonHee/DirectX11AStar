#pragma once

#include "Shader.h"
#include "RTTI.h"
#include "Math/Vector3.h"

struct MaterialBuffer
{
	Vector3 materialColor = Vector3(1.0f, 1.0f, 1.0f); // 기본 흰색
	float materialAlpha = 1.0f; // 알파값 추가
	float useVertexColor = 1.0f; // 기본적으로 버텍스 색상 사용
	float padding[3]; // 16바이트 정렬을 위한 패딩
};

class Engine_API DefaultShader : public Shader
{
	RTTI_DECLARATIONS(DefaultShader, Shader)
public:
	DefaultShader();
	virtual ~DefaultShader();

	// 머티리얼 색상 설정
	void SetMaterialColor(const Vector3& color);
	void SetMaterialColor(const Vector3& color, float alpha);
	void SetUseVertexColor(bool useVertex);

	virtual void Bind() override;

private:
	MaterialBuffer materialData;
	ID3D11Buffer* materialBuffer = nullptr;
};