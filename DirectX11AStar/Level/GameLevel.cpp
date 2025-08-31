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
    pathfinder3D = new AStar3D(gridSize, gridSize, gridSize, 1.2f);

    //// Player �߰� - ��Ȯ�� ����(0,0,0)�� ��ġ
    //player = std::make_shared<Player>();
    //player->transform.position = Vector3(0.0f, 20.0f, 0.0f);  // X,Y,Z���� ������ ����
    //player->transform.scale = Vector3::One * 0.1f;  // �� ũ�� �ؼ� �� ���̵���
    //AddActor(player);

    // CameraActor �߰� - �̹���ó�� ����� �������� ����
    camera = std::make_shared<CameraActor>();
    // �׸��带 �밢������ �ణ ������ �����ٺ��� ���� ��ġ
    camera->transform.position = Vector3(8.0f, 8.0f, -12.0f);

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

    // OŰ �Է� üũ - Open ��� ǥ�� ���
    if (Input::Get().GetKeyDown('O')) {
        showOpenNodes = !showOpenNodes;
    }

    // CŰ �Է� üũ - Closed ��� ǥ�� ���
    if (Input::Get().GetKeyDown('C')) {
        showClosedNodes = !showClosedNodes;
    }
}

void GameLevel::CreateAxisLines()
{
    float lineLength = 15.0f; // �׸��� ũ�⿡ �°� �� ���� ����

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
    // ��ֹ��� �����ϰ� ��ġ 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    gridMeshComponents.clear();
    gridMeshComponents.reserve(gridSize * gridSize * gridSize);

    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            for (int z = 0; z < gridSize; z++) {
                // n% Ȯ���� ��ֹ� ����
                bool isObstacle = dis(gen) < 0.1f;

                // �������� ������ ��ֹ��� ������ ����
                if ((x == startX && y == startY && z == startZ) ||
                    (x == endX && y == endY && z == endZ)) {
                    isObstacle = false;
                }

                pathfinder3D->SetObstacle(x, y, z, isObstacle);

                auto cubeActor = std::make_shared<Actor>();
                auto cubeMeshComponent = std::make_shared<StaticMeshComponent>();

                // ��� ť��� CubeMesh ���
                cubeMeshComponent->SetMesh(std::make_shared<CubeMesh>());

                // �������� ������ �ָ���, �������� ���̾�������
                bool isSpecialNode = (x == startX && y == startY && z == startZ) ||
                    (x == endX && y == endY && z == endZ);
                cubeMeshComponent->SetWireframe(!isSpecialNode);

                // ��ġ ���� (�׸��� �߽��� ��������)
                Vector3 cubePos = pathfinder3D->GridToWorld(x, y, z);
                cubeActor->transform.position = cubePos;
                cubeActor->transform.scale = Vector3(1.0f, 1.0f, 1.0f);
                //// �ʱ� ���� �� ������ ����
                //Vector3 initialColor;
                //float alpha = 1.0f;
                //
                //if (x == startX && y == startY && z == startZ) {
                //    // ������
                //    initialColor = GetNodeColor(NodeState3D::Start);
                //    alpha = 1.0f;
                //} else if (x == endX && y == endY && z == endZ) {
                //    // ����
                //    initialColor = GetNodeColor(NodeState3D::Target);
                //    alpha = 1.0f;
                //} else if (isObstacle) {
                //    // ��ֹ�
                //    initialColor = GetNodeColor(NodeState3D::Obstacle);
                //    alpha = 0.3f;
                //} else {
                //    // �� ����
                //    initialColor = GetNodeColor(NodeState3D::Empty);
                //    alpha = 1.0f;
                //}
                //
                //cubeMeshComponent->SetMaterialColor(initialColor, alpha);

                // ���̴� ����
                std::weak_ptr<DefaultShader> shader;
                if (ShaderLoader::Get().Load<DefaultShader>(shader)) {
                    cubeMeshComponent->AddShader(shader);
                }

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
    // �ɼǿ� ���� Open/Closed ��� ǥ�� ���� ����
    bool shouldShow = true;
    if (state == NodeState3D::Open && !showOpenNodes) {
        shouldShow = false;
    }
    if (state == NodeState3D::Closed && !showClosedNodes) {
        shouldShow = false;
    }

    // �׸��� �ε��� ��� (z * width * height + y * width + x)
    int index = z * gridSize * gridSize + y * gridSize + x;

    if (index >= 0 && index < gridMeshComponents.size()) {
        Vector3 color;
        bool useSolidCube;

        float alpha = 1.0f;

        if (!shouldShow) {
            //ǥ������ ���� ��� ���� ���·� �ǵ���
            //bool isObstacle = !pathfinder3D->GetGridNode(x, y, z).isWalkable;
            //if (isObstacle) {
            //    color = GetNodeColor(NodeState3D::Obstacle);
            //    alpha = 1.0f;
            //} else {
            //    color = GetNodeColor(NodeState3D::Empty);
            //    alpha = 1.0f; // �� ������ �ſ� ����
            //}
            useSolidCube = false;
        }
        else {
            // ���������� ǥ��
            NodeColorInfo colorInfo = GetNodeColorInfo(state, x, y, z);
            color = colorInfo.color;
            alpha = colorInfo.alpha;
            useSolidCube = ShouldUseSolidCube(state);
        }

        gridMeshComponents[index]->SetMaterialColor(color, alpha);

        // ��� ���¿� ���� ���̾������� ����
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
        baseColor = Vector3(1.0f, 1.0f, 1.0f);
        baseAlpha = 1.0f;
        break;
    case NodeState3D::Obstacle:
        baseColor = Vector3(1.0f, 1.0f, 1.0f);
        baseAlpha = 0.0f;
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

    // ��ġ�� ��ȿ�ϸ� �ܰ�/���ο� ���� ���İ� ����
    if (x >= 0 && y >= 0 && z >= 0) {
        bool isOuter = IsOuterSurfaceNode(x, y, z);

        // �߿��� ���� (Start, Target, Path)�� ��ġ�� ������� �׻� ���̰�
        if (state == NodeState3D::Start || state == NodeState3D::Target || state == NodeState3D::Path) {
            return NodeColorInfo(baseColor, baseAlpha);
        }

        // �Ϲ� ������ ���ο� ������ �� �����ϰ�
        if (!isOuter) {
            // ���� ������ �ſ� �����ϰ� ó��
            if (state == NodeState3D::Empty) {
                baseAlpha = 0.0f; // ���� ����
            }
            else if (state == NodeState3D::Obstacle) {
                baseAlpha = 0.00f; // ���� ����
            }
            else {
                baseAlpha *= 0.1f; // ���� ���İ��� 10%�� ����
            }
        }
        else {
            // �ܰ� ������ �ణ �� ���̰�
            if (state == NodeState3D::Empty) {
                baseAlpha = 0.1f; // �ſ� ���� ������
            }
            else if (state == NodeState3D::Obstacle) {
                baseAlpha = 0.1f; // �ణ �� ���̰�
            }
        }
    }

    return NodeColorInfo(baseColor, baseAlpha);
}

bool GameLevel::ShouldUseSolidCube(NodeState3D state)
{
    switch (state) {
    case NodeState3D::Start:
    case NodeState3D::Target:
    case NodeState3D::Path:     // ���� ��� (Ȱ��)
        return true;  // �ָ��� ť�� ���
    case NodeState3D::Obstacle: // ��ֹ�
    case NodeState3D::Closed:   // ó���� ��� (Ȱ��)
    case NodeState3D::Open:     // ���� �������� ��� (Ȱ��)
    case NodeState3D::Empty:
    default:
        return false; // ���̾������� ť�� ���
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
                    if (isObstacle)
                    {
                        nodeInfo = GetNodeColorInfo(NodeState3D::Obstacle, x, y, z);
                    }
                    else
                    {
                        nodeInfo = GetNodeColorInfo(NodeState3D::Empty, x, y, z);
                    }
                    initialColor = nodeInfo.color;
                    alpha = nodeInfo.alpha;
                    useSolidCube = ShouldUseSolidCube(NodeState3D::Obstacle);
                }

                // ����� �޽� Ÿ�� ������Ʈ
                int index = z * gridSize * gridSize + y * gridSize + x;
                if (index >= 0 && index < gridMeshComponents.size())
                {
                    gridMeshComponents[index]->SetMaterialColor(initialColor, alpha);
                    // ���̾������� ���� (�ָ��尡 �ƴϸ� ���̾�������) 
                    // Todo: ��ֹ��̳� �׸��� ���̾� �ָ��� �ݴ��̸� ���⸦ ����
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
    // �̹� ���� ���̸� ����
    if (isPathfindingActive) return;

    // ���� �׸��� ���͵��� ���� ���Ϳ��� ����
    for (auto& cubeActor : gridCubes) {
        auto it = std::find(actors.begin(), actors.end(), cubeActor);
        if (it != actors.end()) {
            actors.erase(it);
        }
    }
    gridCubes.clear();
    gridMeshComponents.clear();

    // ���ο� ���� �������� ���� ����
    GenerateRandomStartEnd();

    // 3D �׸��� ����� (���ο� ���� ��ֹ� ��ġ)
    Create3DGrid();

    // ���ã�� �ð�ȭ �ٽ� ����
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

