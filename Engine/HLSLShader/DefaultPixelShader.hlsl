struct PixelInput
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// ��Ƽ���� ���� ��� ����
cbuffer Material : register(b2)
{
    float3 materialColor;
    float materialAlpha; // ���İ� �߰�
    float useVertexColor; // 0: ��Ƽ���� ���� ���, 1: ���ؽ� ���� ���
    float3 padding; // 16����Ʈ ������ ���� �е�
};

float4 main(PixelInput input) : SV_TARGET
{
    // useVertexColor �÷��׿� ���� ���� ����
    float3 finalColor = lerp(materialColor, input.color, useVertexColor);
    return float4(finalColor, materialAlpha);
}