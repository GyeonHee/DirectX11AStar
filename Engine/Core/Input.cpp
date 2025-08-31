#include "Input.h"
#include <Windows.h>

// static 멤버 정의
Input* Input::instance = nullptr;

Input::Input()
{
	instance = this;
}

void Input::ProcessInput()
{
	// 키 입력 확인
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].isKeyDown
			= GetAsyncKeyState(i) & 0x8000;
	}

	// 마우스 버튼 확인
	mouseStates[0].isButtonDown = GetAsyncKeyState(VK_LBUTTON) & 0x8000; // 좌클릭
	mouseStates[1].isButtonDown = GetAsyncKeyState(VK_RBUTTON) & 0x8000; // 우클릭
	mouseStates[2].isButtonDown = GetAsyncKeyState(VK_MBUTTON) & 0x8000; // 휠클릭

	// 마우스 위치 확인
	POINT cursorPos;
	GetCursorPos(&cursorPos);

	previousMousePosition = mousePosition;
	mousePosition = Vector2((float)cursorPos.x, (float)cursorPos.y);
	mouseDelta = mousePosition - previousMousePosition;
}

void Input::SavePreviousKeyStates()
{
	for (int i = 0; i < 255; ++i)
	{
		keyStates[i].previousKeyDown
			= keyStates[i].isKeyDown;
	}
}

void Input::SavePreviousMouseStates()
{
	for (int i = 0; i < 3; ++i)
	{
		mouseStates[i].previousButtonDown
			= mouseStates[i].isButtonDown;
	}
}

bool Input::GetKey(int keyCode)
{
	return keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyDown(int keyCode)
{
	return !keyStates[keyCode].previousKeyDown
		&& keyStates[keyCode].isKeyDown;
}

bool Input::GetKeyUp(int keyCode)
{
	return keyStates[keyCode].previousKeyDown
		&& !keyStates[keyCode].isKeyDown;
}

bool Input::GetMouseButton(int button)
{
	if (button < 0 || button >= 3) return false;
	return mouseStates[button].isButtonDown;
}

bool Input::GetMouseButtonDown(int button)
{
	if (button < 0 || button >= 3) return false;
	return !mouseStates[button].previousButtonDown
		&& mouseStates[button].isButtonDown;
}

bool Input::GetMouseButtonUp(int button)
{
	if (button < 0 || button >= 3) return false;
	return mouseStates[button].previousButtonDown
		&& !mouseStates[button].isButtonDown;
}

Input& Input::Get()
{
	return *instance;
}