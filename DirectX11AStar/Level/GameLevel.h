#pragma once
#include "Level/Level.h"
#include "Core/AStar3D.h"
#include <vector>
#include <memory>

class Player;
class CameraActor;
class AStar;
class StaticMeshComponent;

// Structure to hold color and alpha information together
struct NodeColorInfo {
	Vector3 color;
	float alpha;

	NodeColorInfo(const Vector3& c, float a) : color(c), alpha(a) {}
	NodeColorInfo() : color(Vector3(1.0f, 1.0f, 1.0f)), alpha(1.0f) {}
};

class GameLevel : public Level
{
	RTTI_DECLARATIONS(GameLevel, Level)
public:
	GameLevel();
	~GameLevel();

	virtual void OnUpdate(float deltaTime) override;

private:
	void CreateAxisLines();      // XYZ �� �׸���
	void Create3DGrid();         // 3D �׸��� �ð�ȭ ����
	void UpdateGridVisualization(int x, int y, int z); // �׸��� �ð�ȭ ������Ʈ
	void UpdateNodeVisualization(int x, int y, int z, NodeState3D state); // ���� ��� ���� ������Ʈ
	void StartPathfindingVisualization(); // ���ã�� �ð�ȭ ����
	void ResetAndRestartVisualization(); // RŰ�� �缳�� �� �����
	void GenerateRandomStartEnd(); // ���� �������� ���� ����
	NodeColorInfo GetNodeColorInfo(NodeState3D state, int x = -1, int y = -1, int z = -1); // ��� ���º� ����� ���İ� ��ȯ
	bool ShouldUseSolidCube(NodeState3D state); // �ָ��� ť�긦 ����ؾ� �ϴ��� �Ǵ�
	bool IsOuterSurfaceNode(int x, int y, int z); // �׸��� �ܰ��鿡 �ִ� ������� Ȯ��

	std::shared_ptr<Player> player;
	std::shared_ptr<CameraActor> camera;
	AStar* pathfinder;
	AStar3D* pathfinder3D;

	// 3D Grid visualization
	std::vector<std::shared_ptr<Actor>> gridCubes;
	std::vector<std::shared_ptr<StaticMeshComponent>> gridMeshComponents; // ���� ������Ʈ��
	int gridSize = 10;  // 30x30x30 grid
	bool isPathfindingActive = false;

	// Random start and end points
	int startX = 0, startY = 0, startZ = 0;
	int endX = 0, endY = 0, endZ = 0;

	// Visualization options
	bool showOpenNodes = true;    // OŰ�� ���
	bool showClosedNodes = true;  // CŰ�� ���
};