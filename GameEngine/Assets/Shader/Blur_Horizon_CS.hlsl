Texture2D<float4> gInputMap : register(t0);
RWTexture2D<float4> gOutputUAV : register(u0);

static const int gBlurRadius = 5;
static const float gWeights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.2f, 0.25f, 0.2f, 0.1f, 0.1f, 0.05f, 0.05f };

#define N 256
#define CacheSize (N + 2 * gBlurRadius)
groupshared float4 gCache[CacheSize];

[numthreads(N, 1, 1)]
void Blur_Horizon_CS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	//
	// Fill local thread storage to reduce bandwidth.  To blur 
	// N pixels, we will need to load N + 2*BlurRadius pixels
	// due to the blur radius.
	//
    
	// This thread group runs N threads.  To get the extra 2*BlurRadius pixels, 
	// have 2*BlurRadius threads sample an extra pixel.
    if (groupThreadID.x < gBlurRadius)
    {
		// Clamp out of bound samples that occur at image borders.
        int x = max(dispatchThreadID.x - gBlurRadius, 0);
        gCache[groupThreadID.x] = gInputMap[int2(x, dispatchThreadID.y)];
    }
    if (groupThreadID.x >= N - gBlurRadius)
    {
		// Clamp out of bound samples that occur at image borders.
        int x = min(dispatchThreadID.x + gBlurRadius, gInputMap.Length.x - 1);
        gCache[groupThreadID.x + 2 * gBlurRadius] = gInputMap[int2(x, dispatchThreadID.y)];
    }

	// Clamp out of bound samples that occur at image borders.
    gCache[groupThreadID.x + gBlurRadius] = gInputMap[min(dispatchThreadID.xy, gInputMap.Length.xy - 1)];

	// Wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();

	//
	// Now blur each pixel.
	//

    float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
    for (int k = -gBlurRadius; k <= gBlurRadius; ++k)
    {
        int l = groupThreadID.x + gBlurRadius + k;

        blurColor += gWeights[k + gBlurRadius] * gCache[l];
    }

    gOutputUAV[dispatchThreadID.xy] = blurColor;
}