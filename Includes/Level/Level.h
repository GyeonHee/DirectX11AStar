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

	// 레벨에 액터를 추가할 때 사용
	void AddActor(std::shared_ptr<Actor> newActor);

	std::shared_ptr<Actor> GetActor(int idx) const;
	const uint32 ActorCount() const;

	std::shared_ptr<Actor> GetCamera() const;
	std::shared_ptr<Actor> GetLight() const;

	virtual void OnInit();
	virtual void OnUpdate(float deltaTime);


protected:
	std::vector<std::shared_ptr<Actor>> actors;

	// 메인 카메라 엑터
	std::shared_ptr<Actor> cameraActor;

	// 라이트 액터
	std::shared_ptr<Actor> lightActor;
};