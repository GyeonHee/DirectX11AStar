#pragma once
#include "Core.h"
#include "Math/Vector2.h"

class Engine_API Input
{
	// friend Ŭ����
	friend class Engine;

	// Ű �Է� Ȯ���� ���� ����ü
	struct KeyState
	{
		// ���� �����ӿ� Ű�� ���ȴ��� ����
		bool isKeyDown = false;
		// ���� �����ӿ� Ű�� ���ȴ��� ����
		bool previousKeyDown = false;
	};

	// ���콺 ��ư ����
	struct MouseState
	{
		bool isButtonDown = false;
		bool previousButtonDown = false;
	};

public:
	Input();

	// Ű Ȯ�� �Լ�
	bool GetKey(int keyCode);
	bool GetKeyDown(int keyCode);
	bool GetKeyUp(int keyCode);

	// ���콺 �Լ�
	bool GetMouseButton(int button); // 0: ��Ŭ��, 1: ��Ŭ��, 2: ��Ŭ��
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
	// Ű �Է� ���� ���� �迭
	KeyState keyStates[255] = {};

	// ���콺 ����
	MouseState mouseStates[3] = {}; // ��, ��, �� ��ư
	Vector2 mousePosition = Vector2(0.0f, 0.0f);
	Vector2 previousMousePosition = Vector2(0.0f, 0.0f);
	Vector2 mouseDelta = Vector2(0.0f, 0.0f);

	static Input* instance;
};