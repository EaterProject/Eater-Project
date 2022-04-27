Texture2D<float4> gInputMap : register(t0);
RWTexture2D<float4> gOutputUAV : register(u0);

static const int gBlurRadius = 5;
static const float gWeights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.2f, 0.25f, 0.2f, 0.1f, 0.1f, 0.05f, 0.05f };

#define N 256
#define CacheSize (N + 2 * gBlurRadius)
groupshared float4 gCache[CacheSize];

[numthreads(1, N, 1)]
void Blur_Vertical_CS(int3 groupThreadID : SV_GroupThreadID,
	int3 dispatchThreadID : SV_DispatchThreadID)
{
	//
	// Fill local thread storage to reduce bandwidth.  To blur 
	// N pixels, we will need to load N + 2*BlurRadius pixels
	// due to the blur radius.
	//

	// This thread group runs N threads.  To get the extra 2*BlurRadius pixels, 
	// have 2*BlurRadius threads sample an extra pixel.
    if (groupThreadID.y < gBlurRadius)
    {
		// Clamp out of bound samples that occur at image borders.
        int y = max(dispatchThreadID.y - gBlurRadius, 0);
        gCache[groupThreadID.y] = gInputMap[int2(dispatchThreadID.x, y)];
    }
    if (groupThreadID.y >= N - gBlurRadius)
    {
		// Clamp out of bound samples that occur at image borders.
        int y = min(dispatchThreadID.y + gBlurRadius, gInputMap.Length.y - 1);
        gCache[groupThreadID.y + 2 * gBlurRadius] = gInputMap[int2(dispatchThreadID.x, y)];
    }

	// Clamp out of bound samples that occur at image borders.
    gCache[groupThreadID.y + gBlurRadius] = gInputMap[min(dispatchThreadID.xy, gInputMap.Length.xy - 1)];


	// Wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();

	//
	// Now blur each pixel.
	//

    float4 blurColor = float4(0, 0, 0, 0);

	[unroll]
    for (int k = -gBlurRadius; k <= gBlurRadius; ++k)
    {
        int l = groupThreadID.y + gBlurRadius + k;

        blurColor += gWeights[k + gBlurRadius] * gCache[l];
    }

    gOutputUAV[dispatchThreadID.xy] = blurColor;
}