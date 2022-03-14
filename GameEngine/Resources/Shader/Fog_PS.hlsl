#include "Output_Header.hlsli"

cbuffer cbFog : register(b0)
{
    float3 gFogColor            : packoffset(c0);
    float gFogStartDepth        : packoffset(c0.w);
    float3 gEyePosW             : packoffset(c1);
    float gTime                 : packoffset(c1.w);
}

Texture2D gOriginMap : register(t0);
Texture2D gPositionRT : register(t1);
Texture3D gNoiseVolume : register(t2);

SamplerState gSamClampLinear : register(s0);
SamplerState gSamWrapLinear : register(s1);

float3 ApplyFog(float3 originalColor, float3 toEye, float noise)
{
    float convertPercent = 0.1f;
    
    // 지정 범위로 변환된 Distance..
    float pixelDistance = convertPercent * (length(gEyePosW - toEye) - gFogStartDepth);
    
    // 지정 범위로 변환된 Height..
    float pixelHeight = convertPercent * toEye.y;
    
    float distanceOffset = min(pow(2.0f, pixelDistance - 8.0f), 1.0f);
    float heightOffset = min(pow(1.2f, -(pixelHeight + 3.0f)), 1.0f);
    
	// 거리 기반 안개 강도 설정..
    float distanceValue = exp(0.01f * pow(pixelDistance - 5.0f, 3.0f));
    float fogDistanceFactor = min(distanceValue, 1.0f);
    
    //if (fogDistanceFactor > 0.5f)
    //    return lerp(originalColor, gFogColor, fogDistanceFactor);
    
	// 높이 기반 안개 강도 설정..
    float heightValue = (pixelHeight * 3.0f) - 0.1f;
    float fogHeightFactor = pow(pow(2.0f, -heightValue), heightValue) * (1.0f - distanceOffset);

	// 두 요소를 결합한 최종 요소..
    float fogFinalFactor = min(fogDistanceFactor * fogHeightFactor * noise, 1.0f) + min(distanceOffset * heightOffset, 1.0f) + 0.01f;
    
    return lerp(originalColor, gFogColor, fogFinalFactor);
}

float4 Fog_PS(ScreenPixelIn pin) : SV_TARGET
{
    float3 outColor = gOriginMap.Sample(gSamClampLinear, pin.Tex);
    float3 position = gPositionRT.Sample(gSamClampLinear, pin.Tex).xyz;
	
    // 현재 Position을 Perlin Noise Texture 크기로 정규화..
    float3 coord = float3(int3(position * 100) % 12800) / 12800.0f;
    
    // 움직이는 효과를 위해 시간대비 이동..
    coord.x += gTime;
    coord.y += gTime;

    // Noise 값
    float noise = gNoiseVolume.SampleLevel(gSamWrapLinear, coord, 0).x;
    
    outColor = any(position) ? ApplyFog(outColor, position, noise) : lerp(outColor, gFogColor, 0.75f);
    
    return float4(outColor, 1.0f);
}
