#include "Component.h"
#include "Actor/Actor.h"

Component::Component()
{
}

Component::~Component()
{
}

void Component::OnInit()
{
}

void Component::OnUpdate(float deltaTime)
{
}


void Component::OnRender(bool isShadowDraw)
{
}



Actor* Component::GetOwner() const
{
    return owner;
}

void Component::SetOwner(Actor* newOwner)
{
    owner = newOwner;
}