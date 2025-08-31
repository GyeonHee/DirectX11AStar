#pragma once

#include "Math/Vector3.h"
#include "Math/Vector2.h"
#include "Actor/Actor.h"
#include <vector>

class StaticMeshComponent;
class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)
public:
	Player();
	~Player();

	virtual void OnInit();
	virtual void OnRender();
	virtual void OnUpdate(float deltaTime);

private:
	std::shared_ptr<StaticMeshComponent> meshComponent;

	// 이동 관련 변수  
	Vector3 targetPosition;
	bool isMovingToTarget;
};