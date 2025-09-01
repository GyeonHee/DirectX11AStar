#pragma once
#include "Core.h"
#include "RTTI.h"
#include <Math/Transform.h>
#include <vector>
#include <memory>
#include <string>

class Level;
class Engine_API Actor : public RTTI
{
	friend class Renderer;
	friend class Level;
	RTTI_DECLARATIONS(Actor, RTTI)
public:
	Actor();
	virtual ~Actor();

	virtual void OnInit();
	virtual void OnUpdate(float deltaTime);
	virtual void OnRender(bool isShadowDraw = false);

	void Destroy();

	// ���ʽ� ����
	void SetOwner(Level* newOwner) { owner = newOwner; }
	Level* GetOwner() { return owner; }

	// ������Ʈ �߰� �Լ�.
	void AddComponent(std::shared_ptr<class Component> newComponent);

	void SetUseRenderTexture(bool newUseRenderTexture);

	bool GetUseRenderTexture() const { return useRenderTexture; }

	// ������Ʈ Get �Լ�.
	template<typename T>
	std::shared_ptr<T> GetComponent()
	{
		std::shared_ptr<T> targetComp;
		for (auto& component : components)
		{
			targetComp = std::dynamic_pointer_cast<T>(component);
			if (targetComp)
			{
				return targetComp;
			}
		}

		return nullptr;
	}

	const bool IsActive() const;
	void SetActive(bool active) { isActive = active; }
	const bool HasInitialized() const;

	const bool IsSkyBox() const { return isSkyBox; }

public:
	// ������ TRS ������ �����ϴ� Ʈ������.
	Transform transform;

protected:

	// ���� ����(���ʽ�)
	Level* owner = nullptr;

	// ������ �̸� (�Ϲ������δ� Hash�� ��ȯ�ؼ� ���).
	std::wstring name = TEXT("Actor");
	// ������ �ʱ�ȭ ����.
	bool hasInitialized = false;
	// ������ Ȱ��ȭ ����.
	bool isActive = true;
	// ���� ����.
	bool hasDestroyed = false;

	// ��ī�̹ڽ� ���� ����.
	bool isSkyBox = false;

	// ������Ʈ �迭.
	std::vector<std::shared_ptr<class Component>> components;

	// ���� �ؽ�ó ��� ����.
	bool useRenderTexture = false;
};