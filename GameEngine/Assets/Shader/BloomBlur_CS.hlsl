#define GAUSSIAN_RADIUS 4

Texture2D<float4> gInputMap : register(t0);
RWTexture2D<float4> gOutputUAV : register(u0);

cbuffer cbBloomBlurOrder : register(b0)
{
    float2 gBlurOrder;
}

static float gWeights[9] = { 0.0135f, 0.0476f, 0.1172f, 0.2011f, 0.2408f, 0.2011f, 0.1172f, 0.0476f, 0.0135f };

[numthreads(8, 8, 1)]
void BloomBlur_CS(uint3 groupID : SV_GroupID, uint3 groupThreadID : SV_GroupThreadID, uint groupIndex : SV_GroupIndex, uint3 dispatchID : SV_DispatchThreadID)
{
    int2 pixel = int2(dispatchID.x, dispatchID.y);

    int2 dir = int2(gBlurOrder.x, gBlurOrder.y);

    float4 accumulatedValue = float4(0.0f, 0.0f, 0.0f, 0.0f);

    for (int i = -GAUSSIAN_RADIUS; i <= GAUSSIAN_RADIUS; i++)
    {
        accumulatedValue += gWeights[i + GAUSSIAN_RADIUS] * gInputMap[pixel + (dir * i)];
    }

    gOutputUAV[pixel] = accumulatedValue;
}
