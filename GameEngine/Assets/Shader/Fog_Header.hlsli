#ifndef FOG_HEADER
#define FOG_HEADER

#include "SamplerState_Header.hlsli"

cbuffer cbFogOption
{
    float3 gFogColor            : packoffset(c0);
    float gFogStartDistance     : packoffset(c0.w);
    float gFogDistanceOffset    : packoffset(c1.x);
    float gFogDistanceValue     : packoffset(c1.y);
    float gFogHeightOffset      : packoffset(c1.z);
    float gFogHeightValue       : packoffset(c1.w);
}

cbuffer cbFogData
{
    float3 gEyePos  : packoffset(c0);
    float gTime     : packoffset(c0.w);
}

Texture3D gNoiseVolume;

float3 ApplyFog(float3 originalColor, float3 toEye, float noise)
{
    float convertPercent = 0.1f;
    
    // 지정 범위로 변환된 Distance..
    float pixelDistance = convertPercent * (length(gEyePos - toEye) - gFogStartDistance);
    
    // 지정 범위로 변환된 Height..
    float pixelHeight = convertPercent * toEye.y;
    
    float distanceOffset = min(pow(2.0f, pixelDistance - gFogDistanceOffset), 1.0f);
    float heightOffset = min(pow(1.2f, -(pixelHeight + gFogHeightOffset)), 1.0f);
    
	// 거리 기반 안개 강도 설정..
    float distanceValue = exp(0.01f * pow(pixelDistance - gFogDistanceValue, 3.0f));
    float fogDistanceFactor = min(distanceValue, 1.0f);
    
    //if (fogDistanceFactor > 0.5f)
    //    return lerp(originalColor, gFogColor, fogDistanceFactor);
    
	// 높이 기반 안개 강도 설정..
    float heightValue = (pixelHeight * gFogHeightValue) - 0.1f;
    float fogHeightFactor = pow(2.0f, -(heightValue * heightValue * 0.1f)) * (1.0f - distanceOffset);

	// 두 요소를 결합한 최종 요소..
    float fogFinalFactor = min(min(fogDistanceFactor * fogHeightFactor * noise, 1.0f) + min(distanceOffset * heightOffset, 1.0f) + 0.01f, 1.0f);
    
    return lerp(originalColor, gFogColor, fogFinalFactor);
}

float3 Fog(in float3 color, in float3 position)
{
    // 현재 Position을 Perlin Noise Texture 크기로 정규화..
    float3 coord = float3(int3(position * 100) % 12800) / 12800.0f;
    
    // 움직이는 효과를 위해 시간대비 이동..
    coord.x += gTime;
    coord.y += gTime;

    // Noise 값
    float noise = gNoiseVolume.SampleLevel(gSamWrapLinear, coord, 0).x;
    
    return ApplyFog(color, position, noise);
}

#endif