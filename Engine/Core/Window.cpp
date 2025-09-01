#include "Window.h"

Window::Window(
	HINSTANCE instance,
	WNDPROC messageProcedure,
	const std::wstring& title,
	uint32 width,
	uint32 height)
	: width(width), height(height), title(title), instance(instance)
{

	// â ������ ���� �Ӽ� ����ü.
	WNDCLASS wc = { };
	wc.lpfnWndProc = messageProcedure;
	wc.hInstance = instance;		// ���α׷� ������.
	wc.lpszClassName = className.c_str();

	// Ŭ���� ���
	if (!RegisterClass(&wc))
	{
		// �޽��� ���#1 - ���â(Output) �̿�.
		OutputDebugStringA("Failed to register a window class\n");

		// �޽��� �ڽ� �̿�.
		MessageBoxA(nullptr, "Failed to register a window", "Error", MB_OK);

		// �ߴ��� ����.
		__debugbreak();
	}
	// ��ġ (���).
	unsigned int xPosition = (GetSystemMetrics(SM_CXSCREEN) - width) / 2;
	unsigned int yPosition = (GetSystemMetrics(SM_CYSCREEN) - height) / 2;

	// â ũ�� ����.
	RECT rect{ 0, 0, (long)width, (long)height };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// â ũ�� �缳��
	unsigned int windowWidth = rect.right - rect.left;
	unsigned int windowHeight = rect.bottom = rect.bottom;


	// ����.
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

	// ���� �����ϸ� ����.
	if (!handle)
	{
		// �޽��� ���#1 - ���â(Output) �̿�.
		OutputDebugStringA("Failed to create a window class\n");

		// �޽��� �ڽ� �̿�.
		MessageBoxA(nullptr, "Failed to create a window", "Error", MB_OK);

		// �ߴ��� ����.
		__debugbreak();
	}

	// ������Ʈ / �����ֱ�.
	ShowWindow(handle, SW_SHOW);
	UpdateWindow(handle);
}

Window::~Window()
{
	// Ŭ���� ��� ����.
	UnregisterClass(className.c_str(), instance);
}

void Window::SetWidthHeight(uint32 width, uint32 height)
{
	this->width = width;
	this->height = height;
}