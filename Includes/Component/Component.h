#pragma once
#include "RTTI.h"

class Engine_API Component : public RTTI
{
	RTTI_DECLARATIONS(Component, RTTI)
public:
	Component();
	~Component();

	virtual void OnInit();
	virtual void OnUpdate(float deltaTime);
	virtual void OnRender(bool isShadowDraw);

	class Actor* GetOwner() const;
	void SetOwner(class Actor* newOwner);

protected:
	class Actor* owner = nullptr;
};