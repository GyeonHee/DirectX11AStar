struct PixelInput
{
    float4 position : SV_POSITION;
    float3 color : COLOR;
    float2 texCoord : TEXCOORD;
    float alpha : ALPHA;
};

float4 main(PixelInput input) : SV_TARGET
{
    return float4(input.color, input.alpha);
}