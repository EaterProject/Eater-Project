#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"

cbuffer cbFogOption : register(b0)
{
    float3 gFogColor            : packoffset(c0);
    float gFogStartDistance     : packoffset(c0.w);
    float gFogDistanceOffset    : packoffset(c1.x);
    float gFogDistanceValue     : packoffset(c1.y);
    float gFogHeightOffset      : packoffset(c1.z);
    float gFogHeightValue       : packoffset(c1.w);
}

cbuffer cbFogData : register(b1)
{
    float3 gEyePos  : packoffset(c0);
    float gTime     : packoffset(c0.w);
}

Texture2D gOriginMap : register(t0);
Texture2D gPositionRT : register(t1);
Texture3D gNoiseVolume : register(t2);

float3 ApplyFog(float3 originalColor, float3 toEye, float noise)
{
    float convertPercent = 0.1f;
    
    // ���� ������ ��ȯ�� Distance..
    float pixelDistance = convertPercent * (length(gEyePos - toEye) - gFogStartDistance);
    
    // ���� ������ ��ȯ�� Height..
    float pixelHeight = convertPercent * toEye.y;
    
    float distanceOffset = min(pow(2.0f, pixelDistance - gFogDistanceOffset), 1.0f);
    float heightOffset = min(pow(1.2f, -(pixelHeight + gFogHeightOffset)), 1.0f);
    
	// �Ÿ� ��� �Ȱ� ���� ����..
    float distanceValue = exp(0.01f * pow(pixelDistance - gFogDistanceValue, 3.0f));
    float fogDistanceFactor = min(distanceValue, 1.0f);
    
    //if (fogDistanceFactor > 0.5f)
    //    return lerp(originalColor, gFogColor, fogDistanceFactor);
    
	// ���� ��� �Ȱ� ���� ����..
    float heightValue = (pixelHeight * gFogHeightValue) - 0.1f;
    float fogHeightFactor = pow(pow(2.0f, -heightValue), heightValue) * (1.0f - distanceOffset);

	// �� ��Ҹ� ������ ���� ���..
    float fogFinalFactor = min(min(fogDistanceFactor * fogHeightFactor * noise, 1.0f) + min(distanceOffset * heightOffset, 1.0f) + 0.01f, 1.0f);
    
    return lerp(originalColor, gFogColor, fogFinalFactor);
}

float4 Fog_PS(ScreenPixelIn pin) : SV_TARGET
{
    float3 outColor = gOriginMap.Sample(gSamClampLinear, pin.Tex);
    float3 position = gPositionRT.Sample(gSamClampLinear, pin.Tex).xyz;
	
    // ���� Position�� Perlin Noise Texture ũ��� ����ȭ..
    float3 coord = float3(int3(position * 100) % 12800) / 12800.0f;
    
    // �����̴� ȿ���� ���� �ð���� �̵�..
    coord.x += gTime;
    coord.y += gTime;

    // Noise ��
    float noise = gNoiseVolume.SampleLevel(gSamWrapLinear, coord, 0).x;
    
    outColor = ApplyFog(outColor, position, noise);
    
    return float4(outColor, 1.0f);
}
