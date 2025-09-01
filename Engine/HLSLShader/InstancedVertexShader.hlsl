struct VertexInput
{
    float3 position : POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    
    // 인스턴스 데이터
    float4x4 instanceWorld : INSTANCE_WORLD;
    float4 instanceColor : INSTANCE_COLOR;
    float instanceAlpha : INSTANCE_ALPHA;
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
    float alpha : ALPHA;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    
    // 인스턴스별 월드 변환 적용
    float4 worldPos = mul(float4(input.position, 1.0f), input.instanceWorld);
    
    // 뷰 변환
    float4 viewPos = mul(worldPos, view);
    
    // 프로젝션 변환
    output.position = mul(viewPos, projection);
    
    // 인스턴스별 색상과 알파 적용
    output.color = input.instanceColor.rgb;
    output.alpha = input.instanceAlpha;
    output.texCoord = input.texCoord;
    
    return output;
}