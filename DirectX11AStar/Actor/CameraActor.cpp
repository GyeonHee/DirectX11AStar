#include "CameraActor.h"
#include "../Engine/Component/CameraComponent.h"
#include "Core/Input.h"
#include "Core/Engine.h"


CameraActor::CameraActor()
{
    // ī�޶� ������Ʈ �߰�
    AddComponent(std::make_shared<CameraComponent>());
}

void CameraActor::OnUpdate(float deltaTime)
{
    super::OnUpdate(deltaTime);

    Move(deltaTime);
    Rotate(deltaTime);
}

void CameraActor::Move(float deltaTime)
{
    // 키보드 입력으로 구체 움직이기 (기존 방식도 유지)
    float keyboardMoveSpeed = 5.0f; // 키보드 이동 속도
    Vector3 moveDirection(0.0f, 0.0f, 0.0f);

    // WASD 키로 이동
    if (Input::Get().GetKey('W') || Input::Get().GetKey('w')) {
        moveDirection.z += 1.0f; // 앞으로
    }
    if (Input::Get().GetKey('S') || Input::Get().GetKey('s')) {
        moveDirection.z -= 1.0f; // 뒤로
    }
    if (Input::Get().GetKey('A') || Input::Get().GetKey('a')) {
        moveDirection.x -= 1.0f; // 왼쪽으로
    }
    if (Input::Get().GetKey('D') || Input::Get().GetKey('d')) {
        moveDirection.x += 1.0f; // 오른쪽으로
    }

    // 위아래 이동 (Q, E 키)
    if (Input::Get().GetKey(VK_LCONTROL))
    {
        moveDirection.y -= 1.0f; // 아래로
    }
    if (Input::Get().GetKey(VK_SPACE))
    {
        moveDirection.y += 1.0f; // 위로
    }

    transform.position = transform.position + (moveDirection * keyboardMoveSpeed * deltaTime);
}

void CameraActor::Rotate(float deltaTime)
{
    // 우클릭 상태 확인
    bool currentRightMouseDown = Input::Get().GetMouseButton(1); // 1 = 우클릭

    if (currentRightMouseDown) {
        Vector2 currentMousePos = Input::Get().GetMousePosition();

        if (isRightMouseDown) {
            // 마우스 이동량 계산
            Vector2 mouseDelta = currentMousePos - lastMousePosition;

            // 마우스 X 이동 -> 카메라 Y축 회전 (좌우 회전)
            // 오른쪽으로 드래그하면 왼쪽을 보도록 (반대 방향)
            cameraYaw += mouseDelta.x * mouseSensitivity;

            // 마우스 Y 이동 -> 카메라 X축 회전 (상하 회전)  
            // 위로 드래그하면 아래를 보도록 (반대 방향)
            cameraPitch += mouseDelta.y * mouseSensitivity;

            // 상하 회전 제한 (너무 많이 회전하지 않도록)
            cameraPitch = std::max(-89.0f, std::min(89.0f, cameraPitch));

            // 카메라 회전 적용
            this->transform.rotation.x = cameraPitch;
            this->transform.rotation.y = cameraYaw;
        }

        lastMousePosition = currentMousePos;
        isRightMouseDown = true;
    }
    else {
        isRightMouseDown = false;
    }
}