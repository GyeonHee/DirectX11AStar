#pragma once

#include "Component.h"
#include "Math/Matrix4.h"

// 카메라 버퍼 (상수 버퍼).
struct CameraBuffer
{
	// 뷰 변환 행렬.
	Matrix4 viewMatrix;

	// 투영 변환 행렬.
	Matrix4 projectionMatrix;

	// 카메라 위치.
	Vector3 cameraPosition;

	// 패딩.
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
	// 카메라 속성.
	CameraBuffer data;
	// Dx buffer.
	ID3D11Buffer* cameraBuffer = nullptr;
};