#pragma once

#include "Core.h"
#include <vector>
#include <memory>
#include "RTTI.h"
#include "Math/Vector3.h"

class Actor;
class Engine_API Level : public RTTI
{
	RTTI_DECLARATIONS(Level, RTTI)
public:
	Level();
	virtual ~Level();

	// ������ ���͸� �߰��� �� ���
	void AddActor(std::shared_ptr<Actor> newActor);

	std::shared_ptr<Actor> GetActor(int idx) const;
	const uint32 ActorCount() const;

	std::shared_ptr<Actor> GetCamera() const;
	std::shared_ptr<Actor> GetLight() const;

	virtual void OnInit();
	virtual void OnUpdate(float deltaTime);
	virtual void OnRenderCustom() {} // 커스텀 렌더링을 위한 가상 함수


protected:
	std::vector<std::shared_ptr<Actor>> actors;

	// ���� ī�޶� ����
	std::shared_ptr<Actor> cameraActor;

	// ����Ʈ ����
	std::shared_ptr<Actor> lightActor;
};