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

	// GPU에 쉐이더 객체를 연결(바인딩)하는 함수.
	virtual void Bind();
	virtual void Unbind();

protected:
	// 쉐이더 이름.
	std::wstring name;

	// DX 리소스.
	ID3D11InputLayout* inputlayout = nullptr;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3DBlob* vertexShaderBuffer = nullptr;

	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* pixelShaderBuffer = nullptr;
};