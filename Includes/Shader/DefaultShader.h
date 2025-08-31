#pragma once

#include "Shader.h"
#include "RTTI.h"
#include "Math/Vector3.h"

struct MaterialBuffer
{
	Vector3 materialColor = Vector3(1.0f, 1.0f, 1.0f); // �⺻ ���
	float materialAlpha = 1.0f; // ���İ� �߰�
	float useVertexColor = 1.0f; // �⺻������ ���ؽ� ���� ���
	float padding[3]; // 16����Ʈ ������ ���� �е�
};

class Engine_API DefaultShader : public Shader
{
	RTTI_DECLARATIONS(DefaultShader, Shader)
public:
	DefaultShader();
	virtual ~DefaultShader();

	// ��Ƽ���� ���� ����
	void SetMaterialColor(const Vector3& color);
	void SetMaterialColor(const Vector3& color, float alpha);
	void SetUseVertexColor(bool useVertex);

	virtual void Bind() override;

private:
	MaterialBuffer materialData;
	ID3D11Buffer* materialBuffer = nullptr;
};