#pragma once
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <cmath>
#include "Core/Core.h"
#include "Math/Vector3.h"

enum class NodeState3D {
    Empty,
    Obstacle,
    Start,
    Target,
    Open,      // In open set (being considered)
    Closed,    // In closed set (already processed)
    Path       // Final path
};

struct GridNode3D {
    int x, y, z;
    float gCost;    // Distance from start node
    float hCost;    // Heuristic distance to target
    float fCost;    // gCost + hCost
    GridNode3D* parent;
    bool isWalkable;
    NodeState3D state;

    GridNode3D(int x = 0, int y = 0, int z = 0)
        : x(x), y(y), z(z), gCost(0), hCost(0), fCost(0), parent(nullptr), isWalkable(true), state(NodeState3D::Empty) {
    }

    bool operator==(const GridNode3D& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct GridNode3DHash {
    std::size_t operator()(const GridNode3D& node) const {
        return std::hash<int>()(node.x) ^
            (std::hash<int>()(node.y) << 1) ^
            (std::hash<int>()(node.z) << 2);
    }
};

struct GridNode3DCompare {
    bool operator()(const GridNode3D* a, const GridNode3D* b) const {
        if (a->fCost == b->fCost) {
            return a->hCost > b->hCost;
        }
        return a->fCost > b->fCost;
    }
};

class Engine_API AStar3D {
public:
    // Callback function for real-time visualization
    using VisualizationCallback = std::function<void(int x, int y, int z, NodeState3D state)>;

    AStar3D(int width, int height, int depth, float nodeSize = 1.0f);
    ~AStar3D();

    // Set obstacle at grid position
    void SetObstacle(int x, int y, int z, bool isObstacle = true);

    // Find path from world position to world position (with real-time visualization)
    std::vector<Vector3> FindPath(const Vector3& startWorld, const Vector3& targetWorld, VisualizationCallback callback = nullptr);

    // Convert world position to grid coordinates
    void WorldToGrid(const Vector3& worldPos, int& gridX, int& gridY, int& gridZ);

    // Convert grid coordinates to world position
    Vector3 GridToWorld(int gridX, int gridY, int gridZ);

    // Get grid dimensions
    int GetWidth() const { return gridWidth; }
    int GetHeight() const { return gridHeight; }
    int GetDepth() const { return gridDepth; }

    // Clear all obstacles
    void ClearObstacles();

    // Get grid node for visualization
    const GridNode3D& GetGridNode(int x, int y, int z) const;

    // Check if grid position is valid
    bool IsValidGridPosition(int x, int y, int z) const;

    // Reset all node states
    void ResetNodeStates();

private:
    int gridWidth;
    int gridHeight;
    int gridDepth;
    float nodeSize;
    std::vector<std::vector<std::vector<GridNode3D>>> grid;

    // A* algorithm helpers
    float CalculateHCost(const GridNode3D& from, const GridNode3D& to);
    float CalculateGCost(const GridNode3D& from, const GridNode3D& to);
    std::vector<GridNode3D*> GetNeighbors(GridNode3D* node);
    std::vector<Vector3> RetracePath(GridNode3D* startNode, GridNode3D* endNode);
};