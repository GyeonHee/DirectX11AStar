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

	// 오너십 설정
	void SetOwner(Level* newOwner) { owner = newOwner; }
	Level* GetOwner() { return owner; }

	// 컴포넌트 추가 함수.
	void AddComponent(std::shared_ptr<class Component> newComponent);

	void SetUseRenderTexture(bool newUseRenderTexture);

	bool GetUseRenderTexture() const { return useRenderTexture; }

	// 컴포넌트 Get 함수.
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
	const bool HasInitialized() const;

	const bool IsSkyBox() const { return isSkyBox; }

public:
	// 액터의 TRS 정보를 관리하는 트랜스폼.
	Transform transform;

protected:

	// 소유 레벨(오너십)
	Level* owner = nullptr;

	// 액터의 이름 (일반적으로는 Hash로 변환해서 사용).
	std::wstring name = TEXT("Actor");
	// 액터의 초기화 여부.
	bool hasInitialized = false;
	// 액터의 활성화 여부.
	bool isActive = true;
	// 삭제 여부.
	bool hasDestroyed = false;

	// 스카이박스 액터 여부.
	bool isSkyBox = false;

	// 컴포넌트 배열.
	std::vector<std::shared_ptr<class Component>> components;

	// 렌더 텍스처 사용 여부.
	bool useRenderTexture = false;
};