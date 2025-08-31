#pragma once

#include "Component.h"
#include "RTTI.h"
#include "Math/Vector3.h"
#include <memory>
#include <vector>

class Engine_API StaticMeshComponent : public Component
{
	RTTI_DECLARATIONS(StaticMeshComponent, Component)
public:
	StaticMeshComponent();
	~StaticMeshComponent();

	virtual void OnRender(bool isShadowDraw) override;

	// �޽� ���� �Լ�
	void SetMesh(std::shared_ptr<class Mesh> newMesh);

	// ���̴� ���� �Լ�
	void AddShader(std::weak_ptr<class Shader> newShader);

	// 색상 설정 함수
	void SetMaterialColor(const Vector3& color);
	void SetMaterialColor(const Vector3& color, float alpha);

	// 와이어프레임 모드 설정 함수
	void SetWireframe(bool enable);

private:
	// �޽�(�𵨸�) ������
	std::shared_ptr<class Mesh> mesh;

	// ���̴� (���� �޽� �� ��ŭ �ʿ�)
	std::vector<std::weak_ptr<class Shader>> shaders;

	// 머티리얼 색상 (알파값 포함)
	Vector3 materialColor = Vector3(1.0f, 1.0f, 1.0f);
	float materialAlpha = 1.0f;

	// 와이어프레임 모드 플래그
	bool isWireframe = false;
};