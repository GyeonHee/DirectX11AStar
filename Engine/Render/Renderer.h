#pragma once

#include "Core.h"
#include "RTTI.h"
#include <memory>

class Engine_API Renderer : public RTTI
{
	RTTI_DECLARATIONS(Renderer, RTTI)
		friend class Engine;

public:
	Renderer(uint32 width, uint32 height, HWND window);
	~Renderer();

	// �׸��� �Լ�
	void OnRender(std::shared_ptr<class Level> level);

	// ũ�� ���� �Լ�
	void OnResize(uint32 width, uint32 height);

	// �޽��� �޸��� �ȱ׸����� �����ϴ� �Լ�
	void CullOn();

	// �޽��� �޸��� �׸����� �����ϴ� �Լ�
	void CullOff();

	// ���������� ������ �����ϴ� �Լ�
	void WireframeOn();

	// ������ ������ �����ϴ� �Լ�
	void WireframeOff();

private:
	// ���̴��� ���ε��� RTV �� SRV�� �����ϴ� �Լ�
	void EmptyRTVsAndSRVs();

	// RenderTargetView/DepthStencilView�� Clear�� �� ����ϴ� �Լ�.
	void Clear(ID3D11RenderTargetView** renderTargetView, float* clearColor, ID3D11DepthStencilView* depthStencilView);

	// RenderTexture�� Draw�ϴ� Pass.
	void DrawToRenderTexturePass(std::shared_ptr<Level>& level);

	// ���� RenderTarget�� Draw�ϴ� Pass.
	void DrawFinalPass(std::shared_ptr<Level>& level);

private:
	// ũ�� ���� ���� Ȯ�� ����.
	bool isResizing = false;

	// ��ġ��
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* context = nullptr;
	IDXGISwapChain* swapChain = nullptr;

	// ����
	ID3D11RenderTargetView* renderTargetView = nullptr;

	// ���� ����
	// DepthStencil (3D������ �ʼ�)
	ID3D11DepthStencilView* depthStencilView = nullptr;

	// �����Ͷ����� ������Ʈ.
	ID3D11RasterizerState* cullFrontState = nullptr;
	ID3D11RasterizerState* cullOnRSState = nullptr;
	ID3D11RasterizerState* wireframeState = nullptr;

	// ������ ������Ʈ
	ID3D11BlendState* blendState = nullptr;       // �ȼ� ���� �ռ�(������, ���� ������) ����

	// ����Ʈ ����
	D3D11_VIEWPORT            viewport;

};