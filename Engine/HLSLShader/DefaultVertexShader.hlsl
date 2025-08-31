struct VertexInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// ConstantBuffer.
cbuffer Transform : register(b0)
{
    matrix worldMatrix;
};

// ConstantBuffer.
cbuffer Camera : register(b1)
{
    matrix view;
    matrix projection;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    // 월드 변환
    float4 worldPos = mul(float4(input.position, 1.0f), worldMatrix);
    
    // 뷰 변환
    float4 viewPos = mul(worldPos, view);
    
    // 프로젝션 변환
    output.position = mul(viewPos, projection);
    
    // 색상과 텍스처 좌표 전달
    output.color = input.color;
    output.texCoord = input.texCoord;
    
    return output;
}