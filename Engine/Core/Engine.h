#pragma once
#include "Core.h"
#include "Input.h"
#include "Resource/ModelLoader.h"
#include "Resource/ShaderLoader.h"
#include <memory>
#include <string>

class Engine_API Engine
{
public:
    Engine(HINSTANCE hInstance, const std::wstring& title, uint32 width, uint32 height);
    virtual ~Engine();

    static Engine& Get();

    static LRESULT CALLBACK MessageProcedure(HWND window,
        uint32 message,
        WPARAM wparam,
        LPARAM lparam);

    void Run();

    // ���� ���� ���� �Լ�
    void SetLevel(std::shared_ptr<class Level> newLevel);

    // ȭ�� ũ�� ���� �̺�Ʈ ���� �Լ�.
    void OnResize(uint32 width, uint32 height);

    // ���� ���� �Լ�.
    void Quit();

    // Getter.
    ID3D11Device& Device() const;
    ID3D11DeviceContext& Context() const;
    class Renderer& GetRenderer() const;

    uint32 Width() const;
    uint32 Height() const;
    

private:
    // void RenderFrame();

protected:
    static Engine* instance;

    // ���� ���� �÷���.
    bool isQuit = false;

    // â ��ü.
    std::shared_ptr<class Window> window;

    // ������ ��ü.
    std::shared_ptr<class Renderer> renderer;

    // �� �δ� ��ü.
    std::unique_ptr<class ModelLoader> modelLoader;

    // ���̴� �δ� ��ü.
    std::unique_ptr<class ShaderLoader> shaderLoader;

    // �Է� �ý��� ��ü.
    std::unique_ptr<class Input> input;

    // ���� ����.
    std::shared_ptr<class Level> mainLevel;
};