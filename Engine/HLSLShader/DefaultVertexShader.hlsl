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
    
    // ���� ��ȯ
    float4 worldPos = mul(float4(input.position, 1.0f), worldMatrix);
    
    // �� ��ȯ
    float4 viewPos = mul(worldPos, view);
    
    // �������� ��ȯ
    output.position = mul(viewPos, projection);
    
    // ����� �ؽ�ó ��ǥ ����
    output.color = input.color;
    output.texCoord = input.texCoord;
    
    return output;
}