#include "DefaultShader.h"
#include "Core/Engine.h"

DefaultShader::DefaultShader()
	: Shader(TEXT("Default"))
{
	// ��Ƽ���� ���� ����
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.ByteWidth = sizeof(MaterialBuffer);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA bufferData = {};
	bufferData.pSysMem = &materialData;

	ID3D11Device& device = Engine::Get().Device();
	HRESULT result = device.CreateBuffer(&bufferDesc, &bufferData, &materialBuffer);
	if (FAILED(result))
	{
		MessageBoxA(nullptr, "Failed to create material buffer", "Error", MB_OK);
	}
}

DefaultShader::~DefaultShader()
{
	if (materialBuffer)
	{
		materialBuffer->Release();
		materialBuffer = nullptr;
	}
}

void DefaultShader::SetMaterialColor(const Vector3& color)
{
	materialData.materialColor = color;
	materialData.materialAlpha = 1.0f; // �⺻���� ������
	materialData.useVertexColor = 0.0f; // ��Ƽ���� ���� ���
}

void DefaultShader::SetMaterialColor(const Vector3& color, float alpha)
{
	materialData.materialColor = color;
	materialData.materialAlpha = alpha;
	materialData.useVertexColor = 0.0f; // ��Ƽ���� ���� ���
}

void DefaultShader::SetUseVertexColor(bool useVertex)
{
	materialData.useVertexColor = useVertex ? 1.0f : 0.0f;
}

void DefaultShader::Bind()
{
	// �θ� Ŭ������ Bind ȣ�� (���̴� ���ε�)
	Shader::Bind();

	// ��Ƽ���� ���� ������Ʈ
	ID3D11DeviceContext& context = Engine::Get().Context();

	D3D11_MAPPED_SUBRESOURCE resource = {};
	context.Map(materialBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &materialData, sizeof(MaterialBuffer));
	context.Unmap(materialBuffer, 0);

	// ��Ƽ���� ���۸� �ȼ� ���̴��� ���ε� (�������� b2)
	context.PSSetConstantBuffers(2, 1, &materialBuffer);
}