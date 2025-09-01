#include "InstancedCubeMesh.h"
#include "Common.h"
#include "Core/Engine.h"

InstancedCubeMesh::InstancedCubeMesh()
{
    // 큐브의 기본 정점 데이터 설정 (CubeMesh와 동일)
    vertices = {
        // 앞면
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

        // 뒷면
        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

        // 왼쪽면
        { { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

        // 오른쪽면
        { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

        // 위면
        { { -0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },

        // 아래면
        { { -0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } },
        { { -0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } },
        { {  0.5f, -0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }
    };

    indices = {
        // 앞면
        0, 1, 2, 2, 3, 0,
        // 뒷면
        4, 5, 6, 6, 7, 4,
        // 왼쪽면
        8, 9, 10, 10, 11, 8,
        // 오른쪽면
        12, 13, 14, 14, 15, 12,
        // 위면
        16, 17, 18, 18, 19, 16,
        // 아래면
        20, 21, 22, 22, 23, 20
    };

    // 버퍼 생성
    CreateBuffers();
}

void InstancedCubeMesh::UpdateInstances(const std::vector<InstanceData>& instanceData)
{
    if (instancedShader)
    {
        instancedShader->UpdateInstanceBuffer(instanceData);
    }
}

void InstancedCubeMesh::RenderInstanced(uint32 instanceCount)
{
    if (!vertexBuffer || !indexBuffer || !instancedShader || instanceCount == 0)
        return;
    
    auto context = Engine::Get().GetRenderer()->GetContext();
    
    // 셰이더 바인드
    instancedShader->Bind();
    
    // 정점 버퍼와 인스턴스 버퍼 설정
    ID3D11Buffer* buffers[] = { vertexBuffer, instancedShader->GetInstanceBuffer() };
    UINT strides[] = { sizeof(Vertex), sizeof(InstanceData) };
    UINT offsets[] = { 0, 0 };
    
    context->IASetVertexBuffers(0, 2, buffers, strides, offsets);
    context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    // 인스턴싱 드로우 콜
    context->DrawIndexedInstanced(static_cast<UINT>(indices.size()), instanceCount, 0, 0, 0);
}

void InstancedCubeMesh::SetInstancedShader(std::shared_ptr<InstancedShader> shader)
{
    instancedShader = shader;
}