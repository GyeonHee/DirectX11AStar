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

    // 메인 레벨 설정 함수
    void SetLevel(std::shared_ptr<class Level> newLevel);

    // 화면 크기 변경 이벤트 대응 함수.
    void OnResize(uint32 width, uint32 height);

    // 엔진 종료 함수.
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

    // 엔진 종료 플래그.
    bool isQuit = false;

    // 창 객체.
    std::shared_ptr<class Window> window;

    // 렌더러 객체.
    std::shared_ptr<class Renderer> renderer;

    // 모델 로더 객체.
    std::unique_ptr<class ModelLoader> modelLoader;

    // 셰이더 로더 객체.
    std::unique_ptr<class ShaderLoader> shaderLoader;

    // 입력 시스템 객체.
    std::unique_ptr<class Input> input;

    // 메인 레벨.
    std::shared_ptr<class Level> mainLevel;
};