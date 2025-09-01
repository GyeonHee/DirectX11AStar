#include "Engine.h"

#include "Window.h"
#include "Level/Level.h"
#include "Actor/Actor.h"
#include "Render/Renderer.h"
#include "Resource/ModelLoader.h"
#include <iostream>

// �̱��� �ν��Ͻ� �ʱ�ȭ
Engine* Engine::instance = nullptr;

// ������ �޽��� ó�� �ݹ�.
LRESULT Engine::MessageProcedure(
    HWND window,
    uint32 message,
    WPARAM wparam,
    LPARAM lparam)
{
    // �Ϻ� ó��.
    switch (message)
    {
        // â ���� �޽���.
    case WM_DESTROY:
        // ���α׷� ���� �̺�Ʈ ����.
        PostQuitMessage(0);
        return 0;

    case WM_QUIT:
        DestroyWindow(window);
        return 0;

        // Ű�ٿ�.
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE)
        {
            // ���� �޽��� �ڽ� ����.
            if (MessageBoxA(nullptr, "Quit?", "Quit Program", MB_YESNO) == IDYES)
            {
                // Yes ��ư ������ â ����.
                DestroyWindow(window);
                return 0;
            }
        }

        return 0;

        // Ű��.
    }

    // �⺻ ó��.
    return DefWindowProc(window, message, wparam, lparam);
}

Engine::Engine(HINSTANCE hInstance, const std::wstring& title, uint32 width, uint32 height)
{
    instance = this;
    window = std::make_shared<Window>(hInstance, MessageProcedure, title, width, height);
    // ������ ����.
    renderer = std::make_shared<Renderer>(width, height, window->Handle());

    modelLoader = std::make_unique<ModelLoader>();

    shaderLoader = std::make_unique<ShaderLoader>();

    // Input �ý��� �ʱ�ȭ
    input = std::make_unique<Input>();
}

Engine::~Engine()
{
}

Engine& Engine::Get()
{
    return *instance;
}

void Engine::Run()
{
    LARGE_INTEGER currentTime = {};
    LARGE_INTEGER previousTime = currentTime;
    LARGE_INTEGER frequency = {};

    QueryPerformanceFrequency(&frequency);

    QueryPerformanceCounter(&currentTime);

    float targetFrameTime = 120.0f; // 120 FPS ��ǥ.
    float oneFrameTime = 1.0f / targetFrameTime; // �� ������ �ð� (�� ����).

    // ����.
    // �޽��� Ȯ�� (���� ����).
    MSG message = {};
    while (message.message != WM_QUIT)
    {
        if (isQuit)
        {
            break;
        }
        // �޽��� Ȯ�� (â �޽��� Ȯ�� ����).
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            // ��ȯ.
            TranslateMessage(&message);

            // ������.
            DispatchMessage(&message);
        }
        else
        {
            QueryPerformanceCounter(&currentTime);

            // Delta Time ���.
            float deltaTime =
                static_cast<float>(currentTime.QuadPart - previousTime.QuadPart)
                / static_cast<float>(frequency.QuadPart);

            if (deltaTime >= oneFrameTime)
            {
                // FPS Ÿ��Ʋ �ٿ� ǥ��.
               /* char buffer[256] = {};
                sprintf_s(buffer, 256, "DirectX_11 | FPS: %d", static_cast<int>(ceil(1.0f / deltaTime)));
                SetWindowTextA(window->handle, buffer);*/

                wchar_t stat[512] = { };
                swprintf_s(stat, 512, TEXT("[%s] - [DeltaTime: %f] [FPS: %d]"),
                    window->Title().c_str(), deltaTime, (int)ceil(1.0f / deltaTime));
                SetWindowText(window->Handle(), stat);

                // RenderFrame();

                 // Input ó��
                input->SavePreviousKeyStates();
                input->SavePreviousMouseStates();
                input->ProcessInput();

                // ���� �����ӿ� �߰� �� ���� ��û�� ���� ó��
                if (mainLevel)
                {
                    static bool isLevelInitialized = false;
                    if (!isLevelInitialized)
                    {
                        mainLevel->OnInit();
                        isLevelInitialized = true;
                    }
                    mainLevel->OnUpdate(deltaTime);
                    renderer->OnRender(mainLevel);
                }

                // ���� �ð� ������Ʈ.
                previousTime = currentTime;
            }
        }
    }
}

void Engine::SetLevel(std::shared_ptr<class Level> newLevel)
{
    mainLevel = newLevel;
}

void Engine::OnResize(uint32 width, uint32 height)
{
    if (!window)
    {
        return;
    }

    if (!renderer)
    {
        return;
    }

    window->SetWidthHeight(width, height);

    RECT rect;
    GetClientRect(window->Handle(), &rect);

    uint32 w = (uint32)(rect.right - rect.left);
    uint32 h = (uint32)(rect.bottom - rect.top);

    renderer->OnResize(w, h);
}

void Engine::Quit()
{
    isQuit = true;
}

ID3D11Device& Engine::Device() const
{
    return *renderer->device;
}

ID3D11DeviceContext& Engine::Context() const
{
    return *renderer->context;
}

Renderer& Engine::GetRenderer() const
{
    return *renderer;
}

uint32 Engine::Width() const
{
    return window->Width();
}

uint32 Engine::Height() const
{
    return window->Height();
}