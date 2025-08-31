#include "Engine.h"

#include "Window.h"
#include "Level/Level.h"
#include "Actor/Actor.h"
#include "Render/Renderer.h"
#include "Resource/ModelLoader.h"
#include <iostream>

// 싱글톤 인스턴스 초기화
Engine* Engine::instance = nullptr;

// 윈도우 메시지 처리 콜백.
LRESULT Engine::MessageProcedure(
    HWND window,
    uint32 message,
    WPARAM wparam,
    LPARAM lparam)
{
    // 일부 처리.
    switch (message)
    {
        // 창 삭제 메시지.
    case WM_DESTROY:
        // 프로그램 종료 이벤트 발행.
        PostQuitMessage(0);
        return 0;

    case WM_QUIT:
        DestroyWindow(window);
        return 0;

        // 키다운.
    case WM_KEYDOWN:
        if (wparam == VK_ESCAPE)
        {
            // 종료 메시지 박스 띄우기.
            if (MessageBoxA(nullptr, "Quit?", "Quit Program", MB_YESNO) == IDYES)
            {
                // Yes 버튼 누르면 창 제거.
                DestroyWindow(window);
                return 0;
            }
        }

        return 0;

        // 키업.
    }

    // 기본 처리.
    return DefWindowProc(window, message, wparam, lparam);
}

Engine::Engine(HINSTANCE hInstance, const std::wstring& title, uint32 width, uint32 height)
{
    instance = this;
    window = std::make_shared<Window>(hInstance, MessageProcedure, title, width, height);
    // 렌더러 생성.
    renderer = std::make_shared<Renderer>(width, height, window->Handle());

    modelLoader = std::make_unique<ModelLoader>();

    shaderLoader = std::make_unique<ShaderLoader>();

    // Input 시스템 초기화
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

    float targetFrameTime = 120.0f; // 120 FPS 목표.
    float oneFrameTime = 1.0f / targetFrameTime; // 한 프레임 시간 (초 단위).

    // 유지.
    // 메시지 확인 (게임 루프).
    MSG message = {};
    while (message.message != WM_QUIT)
    {
        if (isQuit)
        {
            break;
        }
        // 메시지 확인 (창 메시지 확인 로직).
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            // 변환.
            TranslateMessage(&message);

            // 보내기.
            DispatchMessage(&message);
        }
        else
        {
            QueryPerformanceCounter(&currentTime);

            // Delta Time 계산.
            float deltaTime =
                static_cast<float>(currentTime.QuadPart - previousTime.QuadPart)
                / static_cast<float>(frequency.QuadPart);

            if (deltaTime >= oneFrameTime)
            {
                // FPS 타이틀 바에 표시.
               /* char buffer[256] = {};
                sprintf_s(buffer, 256, "DirectX_11 | FPS: %d", static_cast<int>(ceil(1.0f / deltaTime)));
                SetWindowTextA(window->handle, buffer);*/

                wchar_t stat[512] = { };
                swprintf_s(stat, 512, TEXT("[%s] - [DeltaTime: %f] [FPS: %d]"),
                    window->Title().c_str(), deltaTime, (int)ceil(1.0f / deltaTime));
                SetWindowText(window->Handle(), stat);

                // RenderFrame();

                 // Input 처리
                input->SavePreviousKeyStates();
                input->SavePreviousMouseStates();
                input->ProcessInput();

                // 이전 프레임에 추가 및 삭제 요청된 액터 처리
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

                // 이전 시간 업데이트.
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