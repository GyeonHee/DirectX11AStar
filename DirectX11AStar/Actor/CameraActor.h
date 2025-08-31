#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

class CameraActor : public Actor
{
	RTTI_DECLARATIONS(CameraActor, Actor)
public:
	CameraActor();
	~CameraActor() = default;

	virtual void OnUpdate(float deltaTime) override;

	float GetCameraYaw() const { return cameraYaw; }
	void SetCameraYaw(float _cameraYaw) { cameraYaw = _cameraYaw; }
	float GetCameraPitch() const { return cameraPitch; }
	void SetCameraPitch(float _cameraPitch) { cameraPitch = _cameraPitch; }
	Vector2 GetLastMousePosition() const { return lastMousePosition; }
	void SetLastMousePosition(Vector2 _lastMousePosition) { lastMousePosition = _lastMousePosition; }

private:
	void Move(float deltaTime);
	void Rotate(float deltaTime);

private:
	bool isRightMouseDown = false;
	Vector2 lastMousePosition;
	float cameraYaw = 0.0f;     // YÃà È¸Àü (ÁÂ¿ì)
	float cameraPitch = 0.0f;   // XÃà È¸Àü (»óÇÏ)
	float mouseSensitivity = 0.3f;
};