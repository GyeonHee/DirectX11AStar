#include "StaticMeshComponent.h"
#include "Render/Mesh.h"
#include "Core/Engine.h"
#include "Render/Renderer.h"
#include "Shader/Shader.h"
#include "Shader/DefaultShader.h"

StaticMeshComponent::StaticMeshComponent()
{
}

StaticMeshComponent::~StaticMeshComponent()
{
}

void StaticMeshComponent::OnRender(bool isShadowDraw)
{
	super::OnRender(isShadowDraw);

	if (!mesh) {
		return;
	}

	uint32 meshCount = mesh->SubMeshCount();
	if (meshCount == 0) {
		return;
	}
	if (shaders.size() == 0) {
		return;
	}

	for (uint32 i = 0; i < meshCount; ++i)
	{
		auto subMesh = mesh->GetSubMesh(i);

		if (subMesh.lock() && i < shaders.size() && shaders[i].lock())
		{
			static ID3D11DeviceContext& context = Engine::Get().Context();

			// ����Ʈ �缳��
			D3D11_VIEWPORT vp = {};
			vp.TopLeftX = 0.0f;
			vp.TopLeftY = 0.0f;
			vp.Width = (float)Engine::Get().Width();
			vp.Height = (float)Engine::Get().Height();
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			context.RSSetViewports(1, &vp);

			// ���� �޽� ���ε�.
			subMesh.lock()->Bind();

			// �׸��ڸ� �׸��� ���� �ƴϸ� �Ϲ��� ���̴� ���ε�.
			if (!isShadowDraw)
			{
				// ������ ������ ���� �� ���İ� ����
				if (auto shader = std::dynamic_pointer_cast<DefaultShader>(shaders[i].lock())) {
					shader->SetMaterialColor(materialColor, materialAlpha);
					shader->SetUseVertexColor(false);
				}

				// ���̴� ���ε�.
				shaders[i].lock()->Bind();
			}

			// ���̾������� ��忡 ���� �����Ͷ����� ���� ����
			if (isWireframe)
			{
				Engine::Get().GetRenderer().WireframeOn();
			}
			else
			{
				// ������ �⺻ ������ ���·� ���� ����
				static ID3D11RasterizerState* simpleState = nullptr;
				if (!simpleState)
				{
					D3D11_RASTERIZER_DESC desc = {};
					desc.FillMode = D3D11_FILL_SOLID;
					desc.CullMode = D3D11_CULL_BACK;
					desc.FrontCounterClockwise = FALSE;
					desc.DepthBias = 0;
					desc.DepthBiasClamp = 0.0f;
					desc.SlopeScaledDepthBias = 0.0f;
					desc.DepthClipEnable = TRUE;
					desc.ScissorEnable = FALSE;
					desc.MultisampleEnable = FALSE;
					desc.AntialiasedLineEnable = FALSE;
					Engine::Get().Device().CreateRasterizerState(&desc, &simpleState);
				}
				context.RSSetState(simpleState);
			}

			// ���� ��ü�� ���� ���� �׽�Ʈ ����
			if (materialAlpha < 1.0f) {
				// ������ ��ü: ���� �׽�Ʈ�� �ϵ� ���� ����� ��Ȱ��ȭ
				static ID3D11DepthStencilState* transparentDepthState = nullptr;
				if (!transparentDepthState) {
					D3D11_DEPTH_STENCIL_DESC desc = {};
					desc.DepthEnable = TRUE;
					desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // ���� ��Ȱ��ȭ
					desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
					desc.StencilEnable = FALSE;
					Engine::Get().Device().CreateDepthStencilState(&desc, &transparentDepthState);
				}
				context.OMSetDepthStencilState(transparentDepthState, 0);
			}
			else {
				// �������� ��ü: ���� �׽�Ʈ ���� ��Ȱ��ȭ (���� ���� ����)
				static ID3D11DepthStencilState* noDepthState = nullptr;
				if (!noDepthState) {
					D3D11_DEPTH_STENCIL_DESC desc = {};
					desc.DepthEnable = FALSE;
					desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
					desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
					desc.StencilEnable = FALSE;
					Engine::Get().Device().CreateDepthStencilState(&desc, &noDepthState);
				}
				context.OMSetDepthStencilState(noDepthState, 0);
			}

			// ���� ���� ���� ���� (������ ����)
			if (materialAlpha < 1.0f) {
				// ������ ��� ���� ���� Ȱ��ȭ
				static ID3D11BlendState* alphaBlendState = nullptr;
				if (!alphaBlendState) {
					D3D11_BLEND_DESC desc = {};
					desc.RenderTarget[0].BlendEnable = TRUE;
					desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
					desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
					desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
					desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
					desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
					desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
					desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
					Engine::Get().Device().CreateBlendState(&desc, &alphaBlendState);
				}
				float blendFactor[4] = { 1.0f, 1.0f, 1.0f, materialAlpha };
				context.OMSetBlendState(alphaBlendState, blendFactor, 0xffffffff);
			}
			else {
				// �������� ��� ���� ��Ȱ��ȭ
				static ID3D11BlendState* noBlendState = nullptr;
				if (!noBlendState) {
					D3D11_BLEND_DESC desc = {};
					desc.RenderTarget[0].BlendEnable = FALSE;
					desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
					Engine::Get().Device().CreateBlendState(&desc, &noBlendState);
				}
				float blendFactor[4] = { 1.0f, 1.0f, 1.0f, 0.0f };
				context.OMSetBlendState(noBlendState, blendFactor, 0xffffffff);
			}

			// DrawCall.
			uint32 indexCount = subMesh.lock()->IndexCount();
			context.DrawIndexed(indexCount, 0u, 0u);
		}
	}
}

void StaticMeshComponent::SetMesh(std::shared_ptr<Mesh> newMesh)
{
	mesh = newMesh;
}

void StaticMeshComponent::AddShader(std::weak_ptr<Shader> newShader)
{
	shaders.emplace_back(newShader);
}

void StaticMeshComponent::SetMaterialColor(const Vector3& color)
{
	materialColor = color;
	materialAlpha = 1.0f; // �⺻���� ������
}

void StaticMeshComponent::SetMaterialColor(const Vector3& color, float alpha)
{
	materialColor = color;
	materialAlpha = alpha;
}

void StaticMeshComponent::SetWireframe(bool enable)
{
	isWireframe = enable;
}