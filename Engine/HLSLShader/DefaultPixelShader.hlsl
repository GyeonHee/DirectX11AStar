struct PixelInput
{
    float4 position : SV_Position;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
};

// 머티리얼 색상 상수 버퍼
cbuffer Material : register(b2)
{
    float3 materialColor;
    float materialAlpha; // 알파값 추가
    float useVertexColor; // 0: 머티리얼 색상 사용, 1: 버텍스 색상 사용
    float3 padding; // 16바이트 정렬을 위한 패딩
};

float4 main(PixelInput input) : SV_TARGET
{
    // useVertexColor 플래그에 따라 색상 선택
    float3 finalColor = lerp(materialColor, input.color, useVertexColor);
    return float4(finalColor, materialAlpha);
}