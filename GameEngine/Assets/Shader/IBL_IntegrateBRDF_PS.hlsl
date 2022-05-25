#include "Output_Header.hlsli"
#include "IBL_Header.hlsli"

float2 IBL_IntegrateBRDF_PS(ScreenPixelIn pin) : SV_TARGET
{
    float NdotV = pin.Tex.x;
    float roughness = 1.0f - pin.Tex.y;
    float roughness2 = roughness * roughness;
	
	float3 viewDir;
	viewDir.x = 0.0f; // sin
    viewDir.y = NdotV;
    viewDir.z = sqrt(1.0f - (NdotV * NdotV)); // cos

	float A = 0.0f;
	float B = 0.0f;

	float3 normalVec = float3(0.0f, 1.0f, 0.0f);

	const uint numSamples = 4u * 1024u;
	// generates a sample vector that's biased towards the
	// preferred alignment direction (importance sampling)
	for (uint i = 0; i < numSamples; i++)
	{
		float2 Xi = Hammersley(i, numSamples);
        float3 halfwayVec = ImportanceSampleGGX(Xi, roughness2, normalVec);
		float3 lightDir = normalize((2.0f * dot(viewDir, halfwayVec) * halfwayVec) - viewDir);

        float NdotL = max(lightDir.y, 0.0f);
        float NdotH = max(halfwayVec.y, 0.0f);
		float VdotH = max(dot(viewDir, halfwayVec), 0.0f);

		if (NdotL > 0)
		{
            float G = IBLGeometrySmith(normalVec, viewDir, lightDir, roughness2);
            float G_Vis = max((G * VdotH) / (NdotH * NdotV), EPSILON);
            float Fc = pow(1.0f - VdotH, 5.0f);

			A += (1.0f - Fc) * G_Vis;
			B += Fc * G_Vis;
		}
	}
	
    return float2(A, B) / numSamples;
}