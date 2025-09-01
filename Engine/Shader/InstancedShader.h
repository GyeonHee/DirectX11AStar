#pragma once

#include "Shader.h"
#include "Math/Vector3.h"
#include "Math/Matrix4.h"

struct InstanceData
{
    Matrix4 worldMatrix;
    Vector3 color;
    float alpha;
};

class Engine_API InstancedShader : public Shader
{
public:
    InstancedShader();
    virtual ~InstancedShader();

    virtual void Bind() override;
    
    // 인스턴스 데이터 관리
    void UpdateInstanceBuffer(const std::vector<InstanceData>& instanceData);
    ID3D11Buffer* GetInstanceBuffer() const { return instanceBuffer; }

private:
    ID3D11Buffer* instanceBuffer = nullptr;
    size_t instanceBufferSize = 0;
};