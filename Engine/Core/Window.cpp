#include "Window.h"

Window::Window(
	HINSTANCE instance,
	WNDPROC messageProcedure,
	const std::wstring& title,
	uint32 width,
	uint32 height)
	: width(width), height(height), title(title), instance(instance)
{

	// 창 생성을 위한 속성 구조체.
	WNDCLASS wc = { };
	wc.lpfnWndProc = messageProcedure;
	wc.hInstance = instance;		// 프로그램 포인터.
	wc.lpszClassName = className.c_str();

	// 클래스 등록
	if (!RegisterClass(&wc))
	{
		// 메시지 출력#1 - 출력창(Output) 이용.
		OutputDebugStringA("Failed to register a window class\n");

		// 메시지 박스 이용.
		MessageBoxA(nullptr, "Failed to register a window", "Error", MB_OK);

		// 중단점 설정.
		__debugbreak();
	}
	// 위치 (가운데).
	unsigned int xPosition = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	unsigned int yPosition = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	// 창 크기 조정.
	RECT rect{ 0, 0, (long)width, (long)height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// 창 크기 재설정
	unsigned int windowWidth = rect.right - rect.left;
	unsigned int windowHeight = rect.bottom = rect.bottom;


	// 생성.
	handle = CreateWindow(
		className.c_str(),      // Window class
		title.c_str(),          // Window text
		WS_OVERLAPPEDWINDOW,    // Window style
		// Size and position
		xPosition, yPosition,
		windowWidth, windowHeight,
		nullptr,       // Parent window
		nullptr,       // Menu
		instance,  // Instance handle
		nullptr
	);

	// 생성 실패하면 종료.
	if (!handle)
	{
		// 메시지 출력#1 - 출력창(Output) 이용.
		OutputDebugStringA("Failed to create a window class\n");

		// 메시지 박스 이용.
		MessageBoxA(nullptr, "Failed to create a window", "Error", MB_OK);

		// 중단점 설정.
		__debugbreak();
	}

	// 업데이트 / 보여주기.
	ShowWindow(handle, SW_SHOW);
	UpdateWindow(handle);
}

Window::~Window()
{
	// 클래스 등록 해제.
	UnregisterClass(className.c_str(), instance);
}

void Window::SetWidthHeight(uint32 width, uint32 height)
{
	this->width = width;
	this->height = height;
}