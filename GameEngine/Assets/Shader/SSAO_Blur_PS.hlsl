#include "SamplerState_Header.hlsli"

cbuffer cbBlurTexel : register(b0)
{
    float2 gTexelSize	: packoffset(c0.x);
};

cbuffer cbBlurOrder : register(b1)
{
    float2 gBlurOrder : packoffset(c0.x);
};

static float gWeights[9] = { 0.0135f, 0.0476f, 0.1172f, 0.2011f, 0.2408f, 0.2011f, 0.1172f, 0.0476f, 0.0135f };
static int gBlurRadius = 4;

Texture2D gInputMap	: register(t0);
Texture2D gNormalDepthRT : register(t1);

struct VertexIn
{
	float4 PosH  : SV_POSITION;
	float2 Tex   : TEXCOORD;
};

float4 SSAOBlur_PS(VertexIn pin) : SV_Target
{
    float2 texOffset = gTexelSize * gBlurOrder;

	// The center value always contributes to the sum.
    float4 color = gWeights[gBlurRadius] * gInputMap.Sample(gSamClampLinearPoint, pin.Tex);
    float totalWeight = gWeights[gBlurRadius];

    //float4 centerNormalDepth = gNormalDepthRT.Sample(gSamBorderLinearPoint, pin.Tex);
	//
    //if (dot(centerNormalDepth.xyz, centerNormalDepth.xyz) < 0.00001)
    //    return 1.0f;
	
	for (float i = -gBlurRadius; i <= gBlurRadius; ++i)
	{
		// We already added in the center weight.
		if (i == 0)
			continue;

		float2 tex = pin.Tex + i * texOffset;

        //float4 neighborNormalDepth = gNormalDepthRT.Sample(gSamBorderLinearPoint, tex);
		
		//
		// If the center value and neighbor values differ too much (either in 
		// normal or depth), then we assume we are sampling across a discontinuity.
		// We discard such samples from the blur.
		//

        //if (dot(neighborNormalDepth.xyz, centerNormalDepth.xyz) >= 0.8f &&
		//	abs(neighborNormalDepth.w - centerNormalDepth.w) <= 0.2f)
		{
			float weight = gWeights[i + gBlurRadius];

			// Add neighbor pixel to blur.
			color += weight * gInputMap.Sample(gSamClampLinearPoint, tex);

			totalWeight += weight;
		}
	}

	// Compensate for discarded samples by making total weights sum to 1.
	return color / totalWeight;
}