#include "Level.h"
#include "Actor/Actor.h"
#include "Core/Engine.h"
#include "Component/CameraComponent.h"

Level::Level()
{

}
Level::~Level()
{
}
void Level::OnInit()
{
	if (cameraActor)
	{
		cameraActor->OnInit();
	}

	if (lightActor)
	{
		lightActor->OnInit();
	}

	for (const auto& actor : actors)
	{
		actor->OnInit();
	}
}

void Level::OnUpdate(float deltaTime)
{
	if (cameraActor)
	{
		cameraActor->OnUpdate(deltaTime);
	}

	if (lightActor)
	{
		lightActor->OnUpdate(deltaTime);
	}

	for (const auto& actor : actors)
	{
		actor->OnUpdate(deltaTime);
	}
}

void Level::AddActor(std::shared_ptr<Actor> newActor)
{
	// 임시: 모든 Actor를 actors에 추가 (컴포넌트 확인 나중에)
	actors.emplace_back(newActor);

	/*for (auto component : newActor->components)
	{
		// ���� �߰��ϴ� ���Ͱ� ī�޶� ������Ʈ�� �������� Ȯ��
		// �����ٸ�, ���� ī�޶�� ����
		std::shared_ptr<CameraComponent> cameraComp = std::dynamic_pointer_cast<CameraComponent>(component);
		if (cameraComp)
		{
			cameraActor = newActor;
			return;
		}

		//// ���� �߰��ϴ� ���Ͱ� ����Ʈ ������Ʈ�� �����ٸ�, ����Ʈ ���ͷ� ����
		//std::shared_ptr<LightComponent> lightComp = std::dynamic_pointer_cast<LightComponent>(component);
		//if (lightComp)
		//{
		//	lightActor = newActor;
		//	return;
		//}
	}*/
}

std::shared_ptr<Actor> Level::GetActor(int idx) const
{
	// ���� ó��
	if (idx < 0 || idx >= (int)actors.size())
	{
		return nullptr;
	}
	return actors[idx];
}

const uint32 Level::ActorCount() const
{
	return (uint32)actors.size();
}

std::shared_ptr<Actor> Level::GetCamera() const
{
	return cameraActor;
}

std::shared_ptr<Actor> Level::GetLight() const
{
	return lightActor;
}