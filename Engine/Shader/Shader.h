#pragma once

#include "RTTI.h"
#include "Core.h"

#include <string>

class Engine_API Shader : public RTTI
{
	RTTI_DECLARATIONS(Shader, RTTI)
public:
	Shader(const std::wstring& name = L"Default");
	virtual ~Shader();

	// GPU�� ���̴� ��ü�� ����(���ε�)�ϴ� �Լ�.
	virtual void Bind();
	virtual void Unbind();

protected:
	// ���̴� �̸�.
	std::wstring name;

	// DX ���ҽ�.
	ID3D11InputLayout* inputlayout = nullptr;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3DBlob* vertexShaderBuffer = nullptr;

	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* pixelShaderBuffer = nullptr;
};