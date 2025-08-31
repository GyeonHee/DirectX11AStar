#pragma once
#include "Core.h"
#include "Math/Vector2.h"

class Engine_API Input
{
	// friend 클래스
	friend class Engine;

	// 키 입력 확인을 위한 구조체
	struct KeyState
	{
		// 현재 프레임에 키가 눌렸는지 여부
		bool isKeyDown = false;
		// 이전 프레임에 키가 눌렸는지 여부
		bool previousKeyDown = false;
	};

	// 마우스 버튼 상태
	struct MouseState
	{
		bool isButtonDown = false;
		bool previousButtonDown = false;
	};

public:
	Input();

	// 키 확인 함수
	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	// 마우스 함수
	bool GetMouseButton(int button); // 0: 좌클릭, 1: 우클릭, 2: 휠클릭
	bool GetMouseButtonDown(int button);
	bool GetMouseButtonUp(int button);

	Vector2 GetMousePosition() const { return mousePosition; }
	Vector2 GetMouseDelta() const { return mouseDelta; }

	static Input& Get();
private:
	void ProcessInput();
	void SavePreviousKeyStates();
	void SavePreviousMouseStates();
private:
	// 키 입력 상태 저장 배열
	KeyState keyStates[255] = {};

	// 마우스 상태
	MouseState mouseStates[3] = {}; // 좌, 우, 휠 버튼
	Vector2 mousePosition = Vector2(0.0f, 0.0f);
	Vector2 previousMousePosition = Vector2(0.0f, 0.0f);
	Vector2 mouseDelta = Vector2(0.0f, 0.0f);

	static Input* instance;
};