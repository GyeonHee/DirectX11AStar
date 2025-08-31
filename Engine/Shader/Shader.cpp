#include "Shader.h"
#include <d3dcompiler.h>
#include "../Core/Engine.h"

Shader::Shader(const std::wstring& name)
	: name(name)
{
	// 실행 파일 경로 기준으로 셰이더 로딩
	wchar_t exePath[512] = {};
	GetModuleFileNameW(NULL, exePath, 512);

	// 실행 파일명 제거하고 디렉토리 경로만 남기기
	wchar_t* lastSlash = wcsrchr(exePath, L'\\');
	if (lastSlash) {
		*(lastSlash + 1) = L'\0';
	}

	wchar_t path[768] = {};
	swprintf_s(path, 768, L"%s%sVertexShader.cso", exePath, name.c_str());

	// ��ġ ��ü ������.
	ID3D11Device& device = Engine::Get().Device();

	// CSO 로딩.
	auto result = D3DReadFileToBlob(path, &vertexShaderBuffer);
	if (FAILED(result))
	{
		// 디버그: 찾으려는 파일 경로 출력
		char pathA[512];
		wcstombs_s(nullptr, pathA, path, 512);

		// 현재 작업 디렉토리도 확인
		char currentDir[512];
		GetCurrentDirectoryA(512, currentDir);

		char buffer[1024];
		sprintf_s(buffer, "Failed to read vertex shader object\nPath: %s\nCurrent Dir: %s\nHRESULT: 0x%08X", pathA, currentDir, result);
		MessageBoxA(
			nullptr,
			buffer,
			"Error",
			MB_OK
		);

		return; // __debugbreak() 대신 return 사용
	}

	// ���̴� ����.
	result = device.CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		nullptr,
		&vertexShader
	);

	if (FAILED(result))
	{
		MessageBoxA(
			nullptr,
			"Failed to create vertex shader",
			"Error",
			MB_OK
		);

		__debugbreak();
	}

	// �Է� ���̾ƿ�.
	// ���� ���̴��� ������ ���� �����Ͱ� ��� ������� �˷���.
	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	result = device.CreateInputLayout(
		inputDesc,
		_countof(inputDesc),
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&inputlayout
	);

	if (FAILED(result))
	{
		MessageBoxA(
			nullptr,
			"Failed to create input layout",
			"Error",
			MB_OK
		);

		__debugbreak();
	}

	// 픽셀 셰이더 경로 설정
	swprintf_s(path, 768, L"%s%sPixelShader.cso", exePath, name.c_str());

	result = D3DReadFileToBlob(path, &pixelShaderBuffer);
	if (FAILED(result))
	{
		// 디버그: 찾으려는 파일 경로 출력
		char pathA[512];
		wcstombs_s(nullptr, pathA, path, 512);

		// 현재 작업 디렉토리도 확인
		char currentDir[512];
		GetCurrentDirectoryA(512, currentDir);

		char buffer[1024];
		sprintf_s(buffer, "Failed to read pixel shader object\nPath: %s\nCurrent Dir: %s\nHRESULT: 0x%08X", pathA, currentDir, result);
		MessageBoxA(
			nullptr,
			buffer,
			"Error",
			MB_OK
		);

		return; // __debugbreak() 대신 return 사용
	}

	// ���̴� ����.
	result = device.CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		nullptr,
		&pixelShader
	);

	if (FAILED(result))
	{
		MessageBoxA(
			nullptr,
			"Failed to create pixel shader",
			"Error",
			MB_OK
		);

		__debugbreak();
	}
}

Shader::~Shader()
{
	// DX ���ҽ� ����.
	if (inputlayout)
	{
		inputlayout->Release();
	}
	if (vertexShader)
	{
		vertexShader->Release();
	}
	if (vertexShaderBuffer)
	{
		vertexShaderBuffer->Release();
	}
	if (pixelShader)
	{
		pixelShader->Release();
	}
	if (pixelShaderBuffer)
	{
		pixelShaderBuffer->Release();
	}
}

void Shader::Bind()
{
	// ��ġ ����(DeviceContext) ������.
	static ID3D11DeviceContext& context = Engine::Get().Context();

	// �Է� ���̾ƿ� ����.
	context.IASetInputLayout(inputlayout);

	// ������ ��� ����.
	context.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ���̴� ����.
	context.VSSetShader(vertexShader, nullptr, 0);
	context.PSSetShader(pixelShader, nullptr, 0);
}

void Shader::Unbind()
{
	static ID3D11DeviceContext& context = Engine::Get().Context();

	static ID3D11VertexShader* nullVS = nullptr;
	static ID3D11PixelShader* nullPS = nullptr;

	context.VSSetShader(nullVS, nullptr, 0);
	context.PSSetShader(nullPS, nullptr, 0);
}