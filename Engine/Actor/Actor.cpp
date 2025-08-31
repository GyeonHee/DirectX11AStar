#include "Actor.h"
#include "Component/Component.h"

Actor::Actor() : components()
{
	// 명시적 components 벡터 초기화
}
Actor::~Actor()
{

}

void Actor::OnInit()
{
	// ���� ó��.
	if (hasInitialized)
	{
		return;
	}

	// �ʱ�ȭ ����.
	hasInitialized = true;

	// ������Ʈ �Լ� ȣ��.
	for (const auto& component : components)
	{
		component->OnInit();
	}
}

void Actor::OnUpdate(float deltaTime)
{
	// ���� ó��.
	if (!IsActive())
	{
		return;
	}

	// Ʈ������ ������Ʈ.
	transform.Tick();

	// ������Ʈ �Լ� ȣ��.
	for (const auto& component : components)
	{
		component->OnUpdate(deltaTime);
	}
}

void Actor::OnRender(bool isShadowDraw)
{
	// ���� ó��.
	if (!IsActive())
	{
		return;
	}


	// Ʈ������ ���ε� (���̴��� ������ ����).
	transform.Bind();

	// ������Ʈ �Լ� ȣ��.
	for (const auto& component : components)
	{
		component->OnRender(isShadowDraw);
	}
}

void Actor::Destroy()
{

	// ���� ��û �Ǿ��ٰ� ����
	hasDestroyed = true;


}

void Actor::AddComponent(std::shared_ptr<class Component> newComponent)
{
	components.push_back(newComponent);
	newComponent->SetOwner(this);
}

void Actor::SetUseRenderTexture(bool newUseRenderTexture)
{
	useRenderTexture = newUseRenderTexture;
}

const bool Actor::IsActive() const
{
	return isActive && !hasDestroyed;
}

const bool Actor::HasInitialized() const
{
	return hasInitialized;
}