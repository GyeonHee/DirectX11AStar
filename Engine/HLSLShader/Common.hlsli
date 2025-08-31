
// Ambient - Hemispheric ambient lighting.
float3 CalcAmbient(float3 worldNormal)
{
    // Convert [-1,1] to [0,1]
    float up = worldNormal.y * 0.5f + 0.5f;
    
    // Upper hemisphere ambient color.
    float3 ambientUp = float3(0.2, 0.2, 0.2);
    
    // Down hemisphere ambient color.
    float3 ambientDown = float3(0.15, 0.15, 0.15);
    
    // Calculate ambient value.
    float3 ambient = ambientDown + up * ambientUp;
    return ambient;
}

// Lambert cosine law.
float CalcLambert(float3 worldNormal, float3 lightDirection)
{
    return saturate(dot(worldNormal, -lightDirection));
}

// Half Lambert.
float CalcHalfLambert(
    float3 worldNormal,
    float3 lightDirection,
    float param = 2)
{
    float nDotL = CalcLambert(worldNormal, lightDirection);
    return pow(nDotL * 0.5f + 0.5f, param);
}

// Phong.
float CalcPhong(
    float3 worldNormal,
    float3 lightDirection,
    float3 cameraDirection,
    float shineness = 16.0f)
{
    float nDotl = CalcLambert(worldNormal, lightDirection);

    float specular = 0;
    if (nDotl)
    {
        float3 reflection = reflect(lightDirection, worldNormal);
        float3 viewDirection = normalize(cameraDirection);
        float rDotv = saturate(dot(reflection, -viewDirection));
        specular = pow(rDotv, shineness);
    }
    
    return specular;
}

float CalcBlinnPhong(
    float3 worldNormal,
    float3 lightDirection,
    float3 cameraDirection,
    float shineness = 32.0f)
{
    float NoL = CalcLambert(worldNormal, lightDirection);

    float specular = 0;
    if (NoL > 0)
    {
        // Half Vector.
        float3 viewDirection = normalize(cameraDirection);
        float3 halfVector = normalize((-lightDirection) + (-viewDirection));
        
        // nDoth.
        float NoH = saturate(dot(worldNormal, halfVector));
        specular = pow(NoH, shineness);
    }
    
    return specular;
}

// Calculate a shadow factor.
float CalculateShadowFactor(Texture2D shadowMap, SamplerState samplerState, int shadowPCFOption, float shadowBias, float shadowBrightness, float4 lightClipPosition)
{
    // Shadow.
    float currentDepth = lightClipPosition.z / lightClipPosition.w;
    float2 uv = lightClipPosition.xy / lightClipPosition.w;
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;
    
    currentDepth -= shadowBias;
    float shadowFactor = 0.0f;
    
    float shadowDepth = shadowMap.Sample(samplerState, uv).x;
    //if (uv.x >= 0.0f && uv.x <= 1 && uv.y >= 0.0f && uv.y <= 1.0f && (currentDepth > shadowDepth))
    if (currentDepth > shadowDepth)
    {
        shadowFactor = shadowBrightness;
    }
    else
    {
        shadowFactor = 1.0f;
    }
    
    // Shadow map without PCF.
    //if (shadowPCFOption == 0)
    //{
    //    float shadowDepth = shadowMap.Sample(samplerState, uv).x;
    //    if (uv.x >= 0.0f && uv.x <= 1 && uv.y >= 0.0f && uv.y <= 1.0f && (currentDepth > shadowDepth))
    //    {
    //        shadowFactor = shadowBrightness;
    //    }
    //    else
    //    {
    //        shadowFactor = 1.0f;
    //    }
    //}
    
    //// PCF for single texel.
    //// Hard shadow.
    //else if (shadowPCFOption == 1)
    //{
    //    shadowFactor = shadowMap.SampleCmpLevelZero(compareSampler, uv, currentDepth).x;
    //    shadowFactor = clamp(shadowFactor, shadowBrightness, 1.0f);
    //}
    
    //// PCF for 4x4 texel.
    //// Soft shadow.
    //else if (shadowPCFOption == 2)
    //{
    //    const float dx = 1.0f / shadowMapSize.x;
    //    const float2 offsets[9] =
    //    {
    //        float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
    //        float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
    //        float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
    //    };
        
    //    [unroll]
    //    for (int ix = 0; ix < 9; ++ix)
    //    {
    //        shadowFactor += shadowMap.SampleCmpLevelZero(compareSampler, uv + offsets[ix], currentDepth).x;
    //    }
        
    //    shadowFactor /= 9.0f;
    //    shadowFactor = clamp(shadowFactor, shadowBrightness, 1.0f);
    //}
    
    return shadowFactor;
}