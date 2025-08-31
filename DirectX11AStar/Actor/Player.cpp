#include "Player.h"
#include "Engine.h"
#include "Component/StaticMeshComponent.h"
#include "Render/SphereMesh.h"
#include "Render/CubeMesh.h"
#include "Shader/DefaultShader.h"
#include "Resource/ShaderLoader.h"
#include "../Engine/Resource/ShaderLoader.inl"
#include "Core/Input.h"
#include <cmath>
#include <cstdio>

Player::Player()
	: targetPosition(0.0f, 0.0f, 0.0f)
	, isMovingToTarget(false)
{
	// StaticMeshComponent 생성 시도
	try {
		meshComponent = std::make_shared<StaticMeshComponent>();
	}
	catch (...) {
		meshComponent = nullptr;
	}

	// SphereMesh 설정
	if (meshComponent) {
		meshComponent->SetMesh(std::make_shared<CubeMesh>());
	}

	// 셰이더 로딩 활성화
	if (meshComponent) {
		// 기본 노란색 설정
		meshComponent->SetMaterialColor(Vector3(0.9f, 0.9f, 0.0f));

		std::weak_ptr<DefaultShader> shader;
		if (ShaderLoader::Get().Load<DefaultShader>(shader))
		{
			meshComponent->AddShader(shader);
		}
	}

	//// �ؽ�ó �ε� �� ���̴��� ����.
	//std::weak_ptr<Texture> diffuseMap;
	//TextureLoader::Get().Load("5k_earth_day_map.png", diffuseMap);
	//shader.lock()->SetTexture(
	//	NormalMappingShader::ETextureBindType::Diffuse,
	//	diffuseMap
	//);

	//std::weak_ptr<Texture> normalMap;
	//TextureLoader::Get().Load("8k_earth_normal_map.png", normalMap);
	//shader.lock()->SetTexture(
	//	NormalMappingShader::ETextureBindType::NormalMap,
	//	normalMap
	//);

	// transform.rotation.x = -90.0f; // 임시로 회전 제거

	// 타겟 위치 초기화
	targetPosition = transform.position;
	isMovingToTarget = false;
}

Player::~Player()
{

}

void Player::OnInit()
{
	super::OnInit();

	// nullptr 체크 후 AddComponent() 호출
	if (meshComponent)
	{
		AddComponent(meshComponent);
	}
}

void Player::OnRender()
{
	super::OnRender();
}

void Player::OnUpdate(float deltaTime)
{
	super::OnUpdate(deltaTime);
}