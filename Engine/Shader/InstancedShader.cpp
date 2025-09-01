#include "InstancedShader.h"
#include "Common.h"
#include "Core/Engine.h"
#include "Resource/ShaderLoader.h"
#include "Render/Renderer.h"

InstancedShader::InstancedShader()
{
    std::weak_ptr<ShaderLoader> shaderLoader = Engine::Get().GetShaderLoader();
    if (!shaderLoader.expired())
    {
        auto loader = shaderLoader.lock();
        
        // 인스턴싱용 버텍스 셰이더 로드
        vertexShaderBuffer = loader->LoadVertexShader(TEXT("Engine/HLSLShader/InstancedVertexShader.hlsl"));
        if (vertexShaderBuffer)
        {
            HRESULT hr = Engine::Get().GetRenderer()->GetDevice()->CreateVertexShader(
                vertexShaderBuffer->GetBufferPointer(),
                vertexShaderBuffer->GetBufferSize(),
                nullptr,
                &vertexShader);
            
            ThrowIfFailed(hr, TEXT("Failed to create instanced vertex shader"));
        }

        // 인스턴싱용 픽셀 셰이더 로드
        pixelShaderBuffer = loader->LoadPixelShader(TEXT("Engine/HLSLShader/InstancedPixelShader.hlsl"));
        if (pixelShaderBuffer)
        {
            HRESULT hr = Engine::Get().GetRenderer()->GetDevice()->CreatePixelShader(
                pixelShaderBuffer->GetBufferPointer(),
                pixelShaderBuffer->GetBufferSize(),
                nullptr,
                &pixelShader);
            
            ThrowIfFailed(hr, TEXT("Failed to create instanced pixel shader"));
        }

        // 입력 레이아웃 생성 (인스턴싱 포함)
        if (vertexShaderBuffer)
        {
            D3D11_INPUT_ELEMENT_DESC inputElementDescs[] = {
                // 정점 데이터
                {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
                
                // 인스턴스 데이터 (월드 행렬 - 4개의 float4로 구성)
                {"INSTANCE_WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1},
                {"INSTANCE_WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1},
                {"INSTANCE_WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1},
                {"INSTANCE_WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1},
                
                // 인스턴스 색상과 알파
                {"INSTANCE_COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1},
                {"INSTANCE_ALPHA", 0, DXGI_FORMAT_R32_FLOAT, 1, 80, D3D11_INPUT_PER_INSTANCE_DATA, 1}
            };

            HRESULT hr = Engine::Get().GetRenderer()->GetDevice()->CreateInputLayout(
                inputElementDescs,
                ARRAYSIZE(inputElementDescs),
                vertexShaderBuffer->GetBufferPointer(),
                vertexShaderBuffer->GetBufferSize(),
                &inputlayout);
            
            ThrowIfFailed(hr, TEXT("Failed to create instanced input layout"));
        }
    }
}

InstancedShader::~InstancedShader()
{
    if (instanceBuffer)
    {
        instanceBuffer->Release();
        instanceBuffer = nullptr;
    }
}

void InstancedShader::Bind()
{
    auto context = Engine::Get().GetRenderer()->GetContext();
    
    // 셰이더 바인드
    context->VSSetShader(vertexShader, nullptr, 0);
    context->PSSetShader(pixelShader, nullptr, 0);
    context->IASetInputLayout(inputlayout);
}

void InstancedShader::UpdateInstanceBuffer(const std::vector<InstanceData>& instanceData)
{
    if (instanceData.empty()) return;
    
    auto device = Engine::Get().GetRenderer()->GetDevice();
    auto context = Engine::Get().GetRenderer()->GetContext();
    
    // 버퍼 크기 계산
    size_t newBufferSize = instanceData.size() * sizeof(InstanceData);
    
    // 버퍼가 없거나 크기가 부족하면 새로 생성
    if (!instanceBuffer || instanceBufferSize < newBufferSize)
    {
        if (instanceBuffer)
        {
            instanceBuffer->Release();
            instanceBuffer = nullptr;
        }
        
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.ByteWidth = static_cast<UINT>(newBufferSize);
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        
        HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, &instanceBuffer);
        ThrowIfFailed(hr, TEXT("Failed to create instance buffer"));
        
        instanceBufferSize = newBufferSize;
    }
    
    // 데이터 업데이트
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = context->Map(instanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    ThrowIfFailed(hr, TEXT("Failed to map instance buffer"));
    
    memcpy(mappedResource.pData, instanceData.data(), instanceData.size() * sizeof(InstanceData));
    
    context->Unmap(instanceBuffer, 0);
}