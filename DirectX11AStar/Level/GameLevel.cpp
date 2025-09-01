#include "GameLevel.h"
#include "Actor/Player.h"
#include "Engine.h"
#include "Math/Transform.h"
#include "Actor/CameraActor.h"
#include "Render/LineMesh.h"
#include "Render/SphereMesh.h"
#include "Render/CubeMesh.h"
#include "Component/StaticMeshComponent.h"
#include "Shader/DefaultShader.h"
#include "Resource/ShaderLoader.h"
#include "../Engine/Resource/ShaderLoader.inl"
#include "Core/AStar3D.h"
#include <cmath>
#include <vector>
#include <random>
#include <thread>
#include <algorithm>

GameLevel::GameLevel()
{
    // 3D A* pathfinder �ʱ�ȭ (������ ������ �� ����ϰ�)
    pathfinder3D = new AStar3D(gridSize, gridSize, gridSize, 3.0f);

    //// Player �߰� - ��Ȯ�� ����(0,0,0)�� ��ġ
    //player = std::make_shared<Player>();
    //player->transform.position = Vector3(0.0f, 20.0f, 0.0f);  // X,Y,Z���� ������ ����
    //player->transform.scale = Vector3::One * 0.1f;  // �� ũ�� �ؼ� �� ���̵���
    //AddActor(player);

    // CameraActor �߰� - �̹���ó�� ����� �������� ����
    camera = std::make_shared<CameraActor>();
    // �׸��带 �밢������ �ణ ������ �����ٺ��� ���� ��ġ
    camera->transform.position = Vector3(0.0f, 0.0f, -35.0f);

    // �ʱ� ī�޶� ���� �� ���콺 ����
    camera->SetCameraYaw(0.0f);// = 0.0f;
    camera->SetCameraPitch(0.0f);// = 0.0f;
    camera->SetLastMousePosition(Vector2(0.0f, 0.0f)); //= Vector2(0.0f, 0.0f);
    camera->transform.rotation.x = camera->GetCameraPitch();// cameraPitch;
    camera->transform.rotation.y = camera->GetCameraYaw();// cameraYaw;
    camera->transform.rotation.z = 0.0f;
    AddActor(camera);

    // ������ �� ����
    CreateAxisLines();

    // ���� �������� ���� ����
    GenerateRandomStartEnd();

    Create3DGrid();

    // 5�� �� ���ã�� �ð�ȭ ����
    StartPathfindingVisualization();
}

GameLevel::~GameLevel()
{
    delete pathfinder;
    delete pathfinder3D;
}

void GameLevel::OnUpdate(float deltaTime)
{
    // �θ� Ŭ������ OnUpdate ȣ��
    super::OnUpdate(deltaTime);

    // RŰ �Է� üũ - �����
    if (Input::Get().GetKeyDown('R')) {
        ResetAndRestartVisualization();
    }

    // 1키 입력 체크 - 장애물 표시 3단계 토글 (와이어프레임 -> 솔리드 -> 숨김 -> 와이어프레임)
    if (Input::Get().GetKeyDown('1')) {
        switch (obstacleDisplayMode) {
        case ObstacleDisplayMode::Wireframe:
            obstacleDisplayMode = ObstacleDisplayMode::Solid;
            break;
        case ObstacleDisplayMode::Solid:
            obstacleDisplayMode = ObstacleDisplayMode::Hidden;
            break;
        case ObstacleDisplayMode::Hidden:
            obstacleDisplayMode = ObstacleDisplayMode::Wireframe;
            break;
        }
        // 모든 그리드 노드를 다시 업데이트
        RefreshAllNodeVisualization();
    }

    // 2키 입력 체크 - Open 노드 표시 토글
    if (Input::Get().GetKeyDown('2')) {
        showOpenNodes = !showOpenNodes;
        // 모든 그리드 노드를 다시 업데이트
        RefreshAllNodeVisualization();
    }
}

void GameLevel::CreateAxisLines()
{
    float lineLength = 30.0f; // �׸��� ũ�⿡ �°� �� ���� ����

    // X�� ���� (������)
    auto xLineActor = std::make_shared<Actor>();
    auto xLineMeshComponent = std::make_shared<StaticMeshComponent>();
    xLineMeshComponent->SetMesh(std::make_shared<LineMesh>(
        Vector3(-lineLength, 0.0f, 0.0f),
        Vector3(lineLength, 0.0f, 0.0f)
    ));

    // ������ ����
    xLineMeshComponent->SetMaterialColor(Vector3(1.0f, 0.0f, 0.0f));

    // ���̴� ����
    std::weak_ptr<DefaultShader> xShader;
    if (ShaderLoader::Get().Load<DefaultShader>(xShader)) {
        xLineMeshComponent->AddShader(xShader);
    }

    xLineActor->AddComponent(xLineMeshComponent);
    AddActor(xLineActor);

    // Y�� ���� (�ʷϻ�)
    auto yLineActor = std::make_shared<Actor>();
    auto yLineMeshComponent = std::make_shared<StaticMeshComponent>();
    yLineMeshComponent->SetMesh(std::make_shared<LineMesh>(
        Vector3(0.0f, -lineLength, 0.0f),
        Vector3(0.0f, lineLength, 0.0f)
    ));

    // �ʷϻ� ����
    yLineMeshComponent->SetMaterialColor(Vector3(0.0f, 1.0f, 0.0f));

    // ���̴� ����
    std::weak_ptr<DefaultShader> yShader;
    if (ShaderLoader::Get().Load<DefaultShader>(yShader)) {
        yLineMeshComponent->AddShader(yShader);
    }

    yLineActor->AddComponent(yLineMeshComponent);
    AddActor(yLineActor);

    // Z�� ���� (�Ķ���)
    auto zLineActor = std::make_shared<Actor>();
    auto zLineMeshComponent = std::make_shared<StaticMeshComponent>();
    zLineMeshComponent->SetMesh(std::make_shared<LineMesh>(
        Vector3(0.0f, 0.0f, -lineLength),
        Vector3(0.0f, 0.0f, lineLength)
    ));

    // �Ķ��� ����
    zLineMeshComponent->SetMaterialColor(Vector3(0.0f, 0.0f, 1.0f));

    // ���̴� ����
    std::weak_ptr<DefaultShader> zShader;
    if (ShaderLoader::Get().Load<DefaultShader>(zShader)) {
        zLineMeshComponent->AddShader(zShader);
    }

    zLineActor->AddComponent(zLineMeshComponent);
    AddActor(zLineActor);
}

void GameLevel::Create3DGrid()
{
    // 장애물을 랜덤하게 배치 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    gridMeshComponents.clear();
    gridMeshComponents.reserve(gridSize * gridSize * gridSize);
    
    currentNodeStates.clear();
    currentNodeStates.resize(gridSize * gridSize * gridSize, NodeState3D::Empty);

    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                // 15% 확률로 장애물 생성
                bool isObstacle = dis(gen) < 0.15f;

                // 시작점과 도착점은 장애물이 아니도록 설정
                if ((x == startX && y == startY && z == startZ) ||
                    (x == endX && y == endY && z == endZ)) {
                    isObstacle = false;
                }

                pathfinder3D->SetObstacle(x, y, z, isObstacle);

                auto cubeActor = std::make_shared<Actor>();
                auto cubeMeshComponent = std::make_shared<StaticMeshComponent>();

                cubeMeshComponent->SetMesh(std::make_shared<CubeMesh>());

                // 노드 상태에 따른 와이어프레임 설정은 나중에 적용

                // 위치 설정 (그리드 중심을 원점으로)
                Vector3 cubePos = pathfinder3D->GridToWorld(x, y, z);
                cubeActor->transform.position = cubePos;
                cubeActor->transform.scale = Vector3(1.0f, 1.0f, 1.0f);

                std::weak_ptr<DefaultShader> shader;
                if (ShaderLoader::Get().Load<DefaultShader>(shader)) {
                    cubeMeshComponent->AddShader(shader);
                }

                // 노드 상태 설정
                int index = z * gridSize * gridSize + y * gridSize + x;
                NodeState3D nodeState;
                if (x == startX && y == startY && z == startZ) {
                    nodeState = NodeState3D::Start;
                } else if (x == endX && y == endY && z == endZ) {
                    nodeState = NodeState3D::Target;
                } else if (isObstacle) {
                    nodeState = NodeState3D::Obstacle;
                } else {
                    nodeState = NodeState3D::Empty;
                }
                currentNodeStates[index] = nodeState;

                // 토글 상태에 따른 표시 여부 결정
                NodeColorInfo initialColorInfo;
                bool useSolidCube;
                
                if (nodeState == NodeState3D::Obstacle && obstacleDisplayMode == ObstacleDisplayMode::Hidden) {
                    // 장애물이지만 숨김 모드인 경우 Empty로 표시
                    initialColorInfo = GetNodeColorInfo(NodeState3D::Empty, x, y, z);
                    useSolidCube = ShouldUseSolidCube(NodeState3D::Empty);
                } else {
                    // 일반적인 경우 (와이어프레임 또는 솔리드 모드 포함)
                    initialColorInfo = GetNodeColorInfo(nodeState, x, y, z);
                    useSolidCube = ShouldUseSolidCube(nodeState);
                }
                
                cubeMeshComponent->SetMaterialColor(initialColorInfo.color, initialColorInfo.alpha);
                cubeMeshComponent->SetWireframe(!useSolidCube);

                cubeActor->AddComponent(cubeMeshComponent);
                AddActor(cubeActor);
                gridCubes.push_back(cubeActor);
                gridMeshComponents.push_back(cubeMeshComponent);
            }
        }
    }
}

void GameLevel::UpdateGridVisualization(int x, int y, int z)
{
    // �ʿ��� ��� �׸��� �ð�ȭ ������Ʈ ����

}

void GameLevel::UpdateNodeVisualization(int x, int y, int z, NodeState3D state)
{
    // 옵션에 따른 노드 표시 여부 결정
    bool shouldShow = true;
    if (state == NodeState3D::Open && !showOpenNodes) {
        shouldShow = false;
    }
    if (state == NodeState3D::Obstacle && obstacleDisplayMode == ObstacleDisplayMode::Hidden) {
        shouldShow = false;
    }
    if (state == NodeState3D::Closed) {
        shouldShow = false; // Closed 노드는 항상 표시하지 않음
    }

    // 그리드 인덱스 계산 (z * width * height + y * width + x)
    int index = z * gridSize * gridSize + y * gridSize + x;

    if (index >= 0 && index < gridMeshComponents.size()) {
        // 현재 노드 상태 업데이트
        currentNodeStates[index] = state;
        Vector3 color;
        bool useSolidCube;

        float alpha = 1.0f;

        if (!shouldShow) {
            // 표시하지 않는 경우 원래 상태로 되돌림
            bool isObstacle = !pathfinder3D->GetGridNode(x, y, z).isWalkable;
            NodeColorInfo colorInfo;
            if (isObstacle && obstacleDisplayMode != ObstacleDisplayMode::Hidden) {
                colorInfo = GetNodeColorInfo(NodeState3D::Obstacle, x, y, z);
            } else {
                colorInfo = GetNodeColorInfo(NodeState3D::Empty, x, y, z);
            }
            color = colorInfo.color;
            alpha = colorInfo.alpha;
            useSolidCube = false;
        } else {
            // 상태별로 표시
            NodeColorInfo colorInfo = GetNodeColorInfo(state, x, y, z);
            color = colorInfo.color;
            alpha = colorInfo.alpha;
            useSolidCube = ShouldUseSolidCube(state);
        }

        gridMeshComponents[index]->SetMaterialColor(color, alpha);

        // 노드 상태에 따른 와이어프레임 설정
        gridMeshComponents[index]->SetWireframe(!useSolidCube);
    }
}

NodeColorInfo GameLevel::GetNodeColorInfo(NodeState3D state, int x, int y, int z)
{
    // �⺻ ����� ���İ� ����
    Vector3 baseColor;
    float baseAlpha;

    switch (state) {
    case NodeState3D::Empty:
        baseColor = Vector3(0.5f, 0.5f, 0.5f);
        baseAlpha = 1.0f;
        break;
    case NodeState3D::Obstacle:
        baseColor = Vector3(0.5f, 0.0f, 1.0f);
        baseAlpha = 1.0f;
        break;
    case NodeState3D::Start:
        baseColor = Vector3(0.0f, 1.0f, 0.0f);
        baseAlpha = 1.0f;
        break;
    case NodeState3D::Target:
        baseColor = Vector3(1.0f, 0.0f, 0.0f);
        baseAlpha = 1.0f;
        break;
    case NodeState3D::Open:
        baseColor = Vector3(0.0f, 1.0f, 0.0f);
        baseAlpha = 1.0f;
        break;
    case NodeState3D::Closed:
        baseColor = Vector3(1.0f, 0.0f, 0.0f);
        baseAlpha = 0.0f;
        break;
    case NodeState3D::Path:
        baseColor = Vector3(0.0f, 1.0f, 0.0f);
        baseAlpha = 1.0f;
        break;
    default:
        baseColor = Vector3(1.0f, 1.0f, 1.0f);
        baseAlpha = 0.5f;
        break;
    }

    // 위치가 유효하면 외곽/내부에 따라 알파값 조절
    if (x >= 0 && y >= 0 && z >= 0) {
        bool isOuter = IsOuterSurfaceNode(x, y, z);

        // 중요한 노드들 (Start, Target, Path)은 위치에 관계없이 항상 보이게
        if (state == NodeState3D::Start || state == NodeState3D::Target || state == NodeState3D::Path) {
            return NodeColorInfo(baseColor, baseAlpha);
        }

        // 일반 노드들은 내부에 있으면 더 투명하게
        if (!isOuter) {
            // 내부 노드들을 매우 투명하게 처리
            if (state == NodeState3D::Empty) {
                baseAlpha = 0.0f; 
            }
            else if (state == NodeState3D::Obstacle) {
                baseAlpha = 1.0f; 
            }
            //else {
            //    baseAlpha *= 0.1f; // ���� ���İ��� 10%�� ����
            //}
        }
        else {
            // 외곽 노드들
            if (state == NodeState3D::Empty) {
                baseAlpha = 0.1f; // �ſ� ���� ������
            }
            //else if (state == NodeState3D::Obstacle) {
            //    baseAlpha = 0.5f; // �ణ �� ���̰�
            //}
        }
    }

    return NodeColorInfo(baseColor, baseAlpha);
}

bool GameLevel::ShouldUseSolidCube(NodeState3D state)
{
    switch (state) {
    case NodeState3D::Start:    // 시작점
    case NodeState3D::Target:   // 끝점
    case NodeState3D::Path:     // 최종경로
        return true;            // 솔리드
    case NodeState3D::Obstacle: // 장애물
        return (obstacleDisplayMode == ObstacleDisplayMode::Solid);  // 모드에 따라 결정
    case NodeState3D::Closed:   // 클로즈리스트
    case NodeState3D::Open:     // 오픈리스트
    case NodeState3D::Empty:
    default:
        return false;           // 와이어프레임
    }
}

bool GameLevel::IsOuterSurfaceNode(int x, int y, int z)
{
    // �׸����� 6�� �� �ϳ��� ���鿡 ��������� �ܰ� ���
    return (x == 0 || x == gridSize - 1 ||
        y == 0 || y == gridSize - 1 ||
        z == 0 || z == gridSize - 1);
}

void GameLevel::StartPathfindingVisualization()
{
    if (isPathfindingActive) return;

    isPathfindingActive = true;

    // ���ã�� ���� �� ��� ��带 �ʱ� ���·� ����
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                Vector3 initialColor;
                float alpha;
                bool useSolidCube = false;

                if (x == startX && y == startY && z == startZ)
                {
                    // ������
                    NodeColorInfo startInfo = GetNodeColorInfo(NodeState3D::Start, x, y, z);
                    initialColor = startInfo.color;
                    alpha = startInfo.alpha;
                    useSolidCube = true;
                }
                else if (x == endX && y == endY && z == endZ)
                {
                    // ����
                    NodeColorInfo targetInfo = GetNodeColorInfo(NodeState3D::Target, x, y, z);
                    initialColor = targetInfo.color;
                    alpha = targetInfo.alpha;
                    useSolidCube = true;
                }
                else
                {
                    // �Ϲ� ������ ���� ��������
                    bool isObstacle = !pathfinder3D->GetGridNode(x, y, z).isWalkable;
                    NodeColorInfo nodeInfo;
                    
                    if (isObstacle && obstacleDisplayMode != ObstacleDisplayMode::Hidden)
                    {
                        nodeInfo = GetNodeColorInfo(NodeState3D::Obstacle, x, y, z);
                        useSolidCube = ShouldUseSolidCube(NodeState3D::Obstacle);
                    }
                    else
                    {
                        nodeInfo = GetNodeColorInfo(NodeState3D::Empty, x, y, z);
                        useSolidCube = ShouldUseSolidCube(NodeState3D::Empty);
                    }
                    initialColor = nodeInfo.color;
                    alpha = nodeInfo.alpha;
                }

                // ����� �޽� Ÿ�� ������Ʈ
                int index = z * gridSize * gridSize + y * gridSize + x;
                if (index >= 0 && index < gridMeshComponents.size())
                {
                    gridMeshComponents[index]->SetMaterialColor(initialColor, alpha);

                    gridMeshComponents[index]->SetWireframe(!useSolidCube);
                }
            }
        }
    }

    // ���� �����忡�� ���ã�� ���� (�ǽð� �ð�ȭ ����)
    std::thread pathfindingThread([this]() {
        Vector3 startPos = pathfinder3D->GridToWorld(startX, startY, startZ);
        Vector3 endPos = pathfinder3D->GridToWorld(endX, endY, endZ);

        // �ݹ� �Լ��� �ǽð� ������Ʈ
        auto visualizationCallback = [this](int x, int y, int z, NodeState3D state) {
            // ���� �����忡�� ����ǵ��� ť�� �߰�
            // ������ ���� ȣ�� (���߿� ������ �������� ���� ť�� ���� ����)
            UpdateNodeVisualization(x, y, z, state);
            };

        std::vector<Vector3> path = pathfinder3D->FindPath(startPos, endPos, visualizationCallback);

        isPathfindingActive = false;
        });

    pathfindingThread.detach(); // ��׶��忡�� ����
}

void GameLevel::ResetAndRestartVisualization()
{
    // 이미 진행 중이면 리턴
    if (isPathfindingActive) return;

    // 현재 토글 상태 백업 (재시작 후에도 유지하기 위해)
    ObstacleDisplayMode savedObstacleDisplayMode = obstacleDisplayMode;
    bool savedShowOpenNodes = showOpenNodes;

    // 기존 그리드 액터들을 씬에서 제거
    for (auto& cubeActor : gridCubes) {
        auto it = std::find(actors.begin(), actors.end(), cubeActor);
        if (it != actors.end()) {
            actors.erase(it);
        }
    }
    gridCubes.clear();
    gridMeshComponents.clear();
    currentNodeStates.clear();

    // 새로운 랜덤 시작점과 도착점 생성
    GenerateRandomStartEnd();

    // 3D 그리드 재생성 (새로운 랜덤 장애물 배치)
    Create3DGrid();

    // 토글 상태 복원
    obstacleDisplayMode = savedObstacleDisplayMode;
    showOpenNodes = savedShowOpenNodes;

    // 경로찾기 시각화 다시 시작
    StartPathfindingVisualization();
}

void GameLevel::GenerateRandomStartEnd()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, gridSize - 1);

    // ���� ������ ����
    startX = dis(gen);
    startY = dis(gen);
    startZ = dis(gen);

    // ���� ���� ���� (�������� �ٸ� ��ġ�� �� ������ �ݺ�)
    do {
        endX = dis(gen);
        endY = dis(gen);
        endZ = dis(gen);
    } while (startX == endX && startY == endY && startZ == endZ);
}

void GameLevel::RefreshAllNodeVisualization()
{
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                int index = z * gridSize * gridSize + y * gridSize + x;
                if (index >= 0 && index < gridMeshComponents.size()) {
                    NodeState3D currentState = currentNodeStates[index];
                    
                    // 표시 여부 결정
                    bool shouldShow = true;
                    if (currentState == NodeState3D::Open && !showOpenNodes) {
                        shouldShow = false;
                    }
                    if (currentState == NodeState3D::Obstacle && obstacleDisplayMode == ObstacleDisplayMode::Hidden) {
                        shouldShow = false;
                    }
                    if (currentState == NodeState3D::Closed) {
                        shouldShow = false; // Closed 노드는 항상 표시하지 않음
                    }

                    Vector3 color;
                    float alpha;
                    bool useSolidCube;

                    if (!shouldShow) {
                        // 표시하지 않는 경우 기본 상태로 표시
                        NodeColorInfo emptyInfo = GetNodeColorInfo(NodeState3D::Empty, x, y, z);
                        color = emptyInfo.color;
                        alpha = emptyInfo.alpha;
                        useSolidCube = false;
                    } else {
                        // 현재 상태에 맞게 표시
                        NodeColorInfo colorInfo = GetNodeColorInfo(currentState, x, y, z);
                        color = colorInfo.color;
                        alpha = colorInfo.alpha;
                        useSolidCube = ShouldUseSolidCube(currentState);
                    }

                    gridMeshComponents[index]->SetMaterialColor(color, alpha);
                    gridMeshComponents[index]->SetWireframe(!useSolidCube);
                }
            }
        }
    }
}



