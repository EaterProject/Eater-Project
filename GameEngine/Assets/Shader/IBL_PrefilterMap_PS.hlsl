#include "SamplerState_Header.hlsli"
#include "Output_Header.hlsli"
#include "IBL_Header.hlsli"

TextureCube gSkyCube		: register(t0);

cbuffer cbExternalData : register(b0) 
{
	float gRoughness;
}

float4 IBL_PrefilterMap_PS(SkyBoxPixelIn pin) : SV_TARGET
{
    float3 normalVec = normalize(pin.PosL);
	float3 R = normalVec;
	float3 viewDir = R;
    float roughness2 = gRoughness * gRoughness;
	
	float3 PrefilteredColor = float3(0.0f, 0.0f, 0.0f);
	float totalWeight = 0.0f;
	
	const uint NumSamples = 1024u;
	for (uint i = 0; i < NumSamples; i++)
	{
		float2 Xi = Hammersley(i, NumSamples);
        float3 halfwayVec = ImportanceSampleGGX(Xi, roughness2, normalVec);
        float3 lightDir = normalize(2.0f * dot(viewDir, halfwayVec) * halfwayVec - viewDir);
		float NdotL = max(dot(normalVec, lightDir), 0.0f);
		
		if (NdotL > 0)
		{
            float D = NormalDistributionGGXTR(normalVec, halfwayVec, roughness2);
            float NdotH = max(dot(normalVec, halfwayVec), 0.0f);
            float HdotV = max(dot(halfwayVec, viewDir), 0.0f);
            float pdf = (D * NdotH) / (4.0f * HdotV);

			float resolution = 512.0f;
			float saTexel = 4.0f * PI / (6.0f * resolution * resolution);
            float saSample = 1.0f / max((float(NumSamples) * pdf), EPSILON);

            float fMipBias = 1.0f;
            float fMipLevel = gRoughness == 0.0 ? 0.0 : max(0.5 * log2(saSample / saTexel) + fMipBias, 0.0f);
			
            PrefilteredColor += gSkyCube.SampleLevel(gSamWrapLinear, lightDir, fMipLevel).rgb * NdotL;
			totalWeight += NdotL;
		}
	}

    PrefilteredColor /= max(totalWeight, EPSILON);

	return float4(PrefilteredColor, 1.0f);
}