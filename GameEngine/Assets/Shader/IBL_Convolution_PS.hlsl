#include "SamplerState_Header.hlsli"
#include "Define_Header.hlsli"
#include "Output_Header.hlsli"
#include "Function_Header.hlsli"

#ifdef HDRI
Texture2D gSkyCube : register(t0);
#else
TextureCube gSkyCube : register(t0);
#endif

float4 IBL_Convolution_PS(SkyBoxPixelIn pin) : SV_TARGET
{
    float3 normal = normalize(pin.PosL);

	float3 irradiance = float3(0.0f, 0.0f, 0.0f);

	float3 up = float3(0.0f, 1.0f, 0.0f);
	float3 right = cross(up, normal);
	up = cross(normal, right);

	float sampleDelta = 0.025f;
	//float sampleDelta = 0.01f;
	float nrSamples = 0.0f;

    for (float phi = 0.0f; phi < TWO_PI; phi += sampleDelta)
	{
        for (float theta = 0.0f; theta < PI_OVER_TWO; theta += sampleDelta)
		{
			float3 tangentSample = float3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
			float3 sampleVec = (tangentSample.xxx * right) + (tangentSample.yyy * up) + (tangentSample.zzz * normal);
			
		#ifdef HDRI
			float2 UV = SampleSphericalMap(sampleVec);
            irradiance += min(gSkyCube.Sample(gSamWrapLinear, UV).rgb, 100.0f) * cos(theta) * sin(theta);
		#else
            irradiance += min(gSkyCube.Sample(gSamWrapLinear, sampleVec).rgb, 100.0f) * cos(theta) * sin(theta);
		#endif			
			nrSamples++;
        }
	}
	
	irradiance = PI * irradiance / nrSamples;

	return float4(irradiance, 1.0f);
}