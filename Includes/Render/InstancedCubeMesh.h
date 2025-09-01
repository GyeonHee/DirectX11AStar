#pragma once
#include "Mesh.h"
#include "Shader/InstancedShader.h"
#include <vector>

class Engine_API InstancedCubeMesh : public Mesh
{
public:
    InstancedCubeMesh();
    ~InstancedCubeMesh() = default;
    
    // 인스턴스 데이터 업데이트
    void UpdateInstances(const std::vector<InstanceData>& instanceData);
    
    // 인스턴싱 렌더링
    void RenderInstanced(uint32 instanceCount);
    
    // 인스턴싱 셰이더 설정
    void SetInstancedShader(std::shared_ptr<InstancedShader> shader);

private:
    std::shared_ptr<InstancedShader> instancedShader;
};