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
	void CreateAxisLines();      // XYZ 축 그리기
	void Create3DGrid();         // 3D 그리드 시각화 생성
	void UpdateGridVisualization(int x, int y, int z); // 그리드 시각화 업데이트
	void UpdateNodeVisualization(int x, int y, int z, NodeState3D state); // 개별 노드 색상 업데이트
	void StartPathfindingVisualization(); // 경로찾기 시각화 시작
	void ResetAndRestartVisualization(); // R키로 재설정 및 재시작
	void GenerateRandomStartEnd(); // 랜덤 시작점과 끝점 생성
	NodeColorInfo GetNodeColorInfo(NodeState3D state, int x = -1, int y = -1, int z = -1); // 노드 상태별 색상과 알파값 반환
	bool ShouldUseSolidCube(NodeState3D state); // 솔리드 큐브를 사용해야 하는지 판단
	bool IsOuterSurfaceNode(int x, int y, int z); // 그리드 외곽면에 있는 노드인지 확인

	std::shared_ptr<Player> player;
	std::shared_ptr<CameraActor> camera;
	AStar* pathfinder;
	AStar3D* pathfinder3D;

	// 3D Grid visualization
	std::vector<std::shared_ptr<Actor>> gridCubes;
	std::vector<std::shared_ptr<StaticMeshComponent>> gridMeshComponents; // 색상 업데이트용
	int gridSize = 10;  // 30x30x30 grid
	bool isPathfindingActive = false;

	// Random start and end points
	int startX = 0, startY = 0, startZ = 0;
	int endX = 0, endY = 0, endZ = 0;

	// Visualization options
	bool showOpenNodes = true;    // O키로 토글
	bool showClosedNodes = true;  // C키로 토글
};