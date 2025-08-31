#pragma once

#include "Component.h"
#include "Math/Matrix4.h"

// ī�޶� ���� (��� ����).
struct CameraBuffer
{
	// �� ��ȯ ���.
	Matrix4 viewMatrix;

	// ���� ��ȯ ���.
	Matrix4 projectionMatrix;

	// ī�޶� ��ġ.
	Vector3 cameraPosition;

	// �е�.
	float padding;
};

class Engine_API CameraComponent : public Component
{
	RTTI_DECLARATIONS(CameraComponent, Component)
public:
	CameraComponent();
	~CameraComponent() = default;

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender(bool isShadowDraw) override;

private:
	// ī�޶� �Ӽ�.
	CameraBuffer data;
	// Dx buffer.
	ID3D11Buffer* cameraBuffer = nullptr;
};