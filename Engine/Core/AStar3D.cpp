#include "Core/AStar3D.h"
#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>

AStar3D::AStar3D(int width, int height, int depth, float nodeSize)
    : gridWidth(width), gridHeight(height), gridDepth(depth), nodeSize(nodeSize) {

    grid.resize(gridWidth);
    for (int x = 0; x < gridWidth; x++) {
        grid[x].resize(gridHeight);
        for (int y = 0; y < gridHeight; y++) {
            grid[x][y].resize(gridDepth);
            for (int z = 0; z < gridDepth; z++) {
                grid[x][y][z] = GridNode3D(x, y, z);
            }
        }
    }
}

AStar3D::~AStar3D() {
}

void AStar3D::SetObstacle(int x, int y, int z, bool isObstacle) {
    if (IsValidGridPosition(x, y, z)) {
        grid[x][y][z].isWalkable = !isObstacle;
        grid[x][y][z].state = isObstacle ? NodeState3D::Obstacle : NodeState3D::Empty;
    }
}

std::vector<Vector3> AStar3D::FindPath(const Vector3& startWorld, const Vector3& targetWorld, VisualizationCallback callback) {
    int startX, startY, startZ;
    int targetX, targetY, targetZ;

    WorldToGrid(startWorld, startX, startY, startZ);
    WorldToGrid(targetWorld, targetX, targetY, targetZ);

    if (!IsValidGridPosition(startX, startY, startZ) ||
        !IsValidGridPosition(targetX, targetY, targetZ) ||
        !grid[targetX][targetY][targetZ].isWalkable) {
        return std::vector<Vector3>();
    }

    // Reset all nodes
    ResetNodeStates();
    for (int x = 0; x < gridWidth; x++) {
        for (int y = 0; y < gridHeight; y++) {
            for (int z = 0; z < gridDepth; z++) {
                grid[x][y][z].gCost = 0;
                grid[x][y][z].hCost = 0;
                grid[x][y][z].fCost = 0;
                grid[x][y][z].parent = nullptr;
            }
        }
    }

    std::priority_queue<GridNode3D*, std::vector<GridNode3D*>, GridNode3DCompare> openSet;
    std::unordered_set<GridNode3D*> openSetLookup; // For faster lookup
    std::unordered_set<GridNode3D*> closedSet;

    GridNode3D* startNode = &grid[startX][startY][startZ];
    GridNode3D* targetNode = &grid[targetX][targetY][targetZ];

    // Set start and target states
    startNode->state = NodeState3D::Start;
    targetNode->state = NodeState3D::Target;

    if (callback) {
        callback(startX, startY, startZ, NodeState3D::Start);
        callback(targetX, targetY, targetZ, NodeState3D::Target);
    }

    startNode->hCost = CalculateHCost(*startNode, *targetNode);
    startNode->fCost = startNode->gCost + startNode->hCost;

    openSet.push(startNode);
    openSetLookup.insert(startNode);

    while (!openSet.empty()) {
        GridNode3D* currentNode = openSet.top();
        openSet.pop();
        openSetLookup.erase(currentNode);

        closedSet.insert(currentNode);

        // Update visualization - mark as closed (processed)
        if (currentNode != startNode && currentNode != targetNode) {
            currentNode->state = NodeState3D::Closed;
            if (callback) {
                callback(currentNode->x, currentNode->y, currentNode->z, NodeState3D::Closed);
            }
        }

        if (currentNode == targetNode) {
            // Path found! Now animate the final path
            std::vector<Vector3> path = RetracePath(startNode, targetNode);
            if (callback) {
                // 1. First, reset all open/closed nodes back to empty state
                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Small delay before cleanup

                // Reset visualization for all nodes except start/target
                for (int x = 0; x < gridWidth; x++) {
                    for (int y = 0; y < gridHeight; y++) {
                        for (int z = 0; z < gridDepth; z++) {
                            GridNode3D& node = grid[x][y][z];
                            if (node.state == NodeState3D::Open || node.state == NodeState3D::Closed) {
                                node.state = NodeState3D::Empty;
                                callback(x, y, z, NodeState3D::Empty);
                            }
                        }
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Pause before showing path

                // 2. Now animate the final path step by step (from start to end)
                // First, collect all path nodes in reverse order
                std::vector<GridNode3D*> pathNodes;
                GridNode3D* pathNode = targetNode->parent;
                while (pathNode != startNode && pathNode != nullptr) {
                    pathNodes.push_back(pathNode);
                    pathNode = pathNode->parent;
                }

                // Now animate from start to end by iterating in reverse
                for (int i = pathNodes.size() - 1; i >= 0; i--) {
                    pathNodes[i]->state = NodeState3D::Path;
                    callback(pathNodes[i]->x, pathNodes[i]->y, pathNodes[i]->z, NodeState3D::Path);
                    std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Slower animation for path
                }
            }
            return path;
        }

        for (GridNode3D* neighbor : GetNeighbors(currentNode)) {
            if (!neighbor->isWalkable || closedSet.find(neighbor) != closedSet.end()) {
                continue;
            }

            float newGCost = currentNode->gCost + CalculateGCost(*currentNode, *neighbor);

            if (newGCost < neighbor->gCost || neighbor->gCost == 0) {
                neighbor->gCost = newGCost;
                neighbor->hCost = CalculateHCost(*neighbor, *targetNode);
                neighbor->fCost = neighbor->gCost + neighbor->hCost;
                neighbor->parent = currentNode;

                if (openSetLookup.find(neighbor) == openSetLookup.end()) {
                    openSet.push(neighbor);
                    openSetLookup.insert(neighbor);

                    // Update visualization - mark as open (being considered)
                    if (neighbor != startNode && neighbor != targetNode) {
                        neighbor->state = NodeState3D::Open;
                        if (callback) {
                            callback(neighbor->x, neighbor->y, neighbor->z, NodeState3D::Open);
                        }
                    }
                }
            }
        }

        // Add delay for visualization to show the process more clearly
        if (callback) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
    }

    return std::vector<Vector3>();
}

void AStar3D::WorldToGrid(const Vector3& worldPos, int& gridX, int& gridY, int& gridZ) {
    gridX = static_cast<int>((worldPos.x + (gridWidth * nodeSize) / 2) / nodeSize);
    gridY = static_cast<int>((worldPos.y + (gridHeight * nodeSize) / 2) / nodeSize);
    gridZ = static_cast<int>((worldPos.z + (gridDepth * nodeSize) / 2) / nodeSize);

    gridX = std::max(0, std::min(gridX, gridWidth - 1));
    gridY = std::max(0, std::min(gridY, gridHeight - 1));
    gridZ = std::max(0, std::min(gridZ, gridDepth - 1));
}

Vector3 AStar3D::GridToWorld(int gridX, int gridY, int gridZ) {
    Vector3 worldPos;
    worldPos.x = (gridX * nodeSize) - (gridWidth * nodeSize) / 2 + nodeSize / 2;
    worldPos.y = (gridY * nodeSize) - (gridHeight * nodeSize) / 2 + nodeSize / 2;
    worldPos.z = (gridZ * nodeSize) - (gridDepth * nodeSize) / 2 + nodeSize / 2;
    return worldPos;
}

void AStar3D::ClearObstacles() {
    for (int x = 0; x < gridWidth; x++) {
        for (int y = 0; y < gridHeight; y++) {
            for (int z = 0; z < gridDepth; z++) {
                grid[x][y][z].isWalkable = true;
            }
        }
    }
}

const GridNode3D& AStar3D::GetGridNode(int x, int y, int z) const {
    return grid[x][y][z];
}

bool AStar3D::IsValidGridPosition(int x, int y, int z) const {
    return x >= 0 && x < gridWidth && y >= 0 && y < gridHeight && z >= 0 && z < gridDepth;
}

float AStar3D::CalculateHCost(const GridNode3D& from, const GridNode3D& to) {
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);
    int dz = abs(to.z - from.z);

    // 3D Euclidean distance
    return static_cast<float>(sqrt(dx * dx + dy * dy + dz * dz));
}

float AStar3D::CalculateGCost(const GridNode3D& from, const GridNode3D& to) {
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);
    int dz = abs(to.z - from.z);

    // Diagonal cost calculation for 3D
    if (dx + dy + dz > 1) {
        return 1.414f; // Diagonal movement cost
    }
    return 1.0f; // Straight movement cost
}

std::vector<GridNode3D*> AStar3D::GetNeighbors(GridNode3D* node) {
    std::vector<GridNode3D*> neighbors;

    // 26 neighbors in 3D space (excluding the center node itself)
    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            for (int z = -1; z <= 1; z++) {
                if (x == 0 && y == 0 && z == 0) continue; // Skip center node

                int checkX = node->x + x;
                int checkY = node->y + y;
                int checkZ = node->z + z;

                if (IsValidGridPosition(checkX, checkY, checkZ)) {
                    neighbors.push_back(&grid[checkX][checkY][checkZ]);
                }
            }
        }
    }

    return neighbors;
}

std::vector<Vector3> AStar3D::RetracePath(GridNode3D* startNode, GridNode3D* endNode) {
    std::vector<Vector3> path;
    GridNode3D* currentNode = endNode;

    while (currentNode != startNode) {
        Vector3 worldPos = GridToWorld(currentNode->x, currentNode->y, currentNode->z);
        path.push_back(worldPos);
        currentNode = currentNode->parent;
    }

    Vector3 startWorldPos = GridToWorld(startNode->x, startNode->y, startNode->z);
    path.push_back(startWorldPos);

    std::reverse(path.begin(), path.end());
    return path;
}

void AStar3D::ResetNodeStates() {
    for (int x = 0; x < gridWidth; x++) {
        for (int y = 0; y < gridHeight; y++) {
            for (int z = 0; z < gridDepth; z++) {
                if (grid[x][y][z].isWalkable) {
                    grid[x][y][z].state = NodeState3D::Empty;
                }
                else {
                    grid[x][y][z].state = NodeState3D::Obstacle;
                }
            }
        }
    }
}