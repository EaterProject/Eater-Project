Texture2D gInputMap : register(t0);
RWTexture2D<float4> gOutputUAV : register(u0);

static const int gBlurRadius = 5;
static const float gWeights[11] = { 0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f };

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
    
    int2 length = int2(0, 0);
    gInputMap.GetDimensions(length.x, length.y);
    length.x -= 1;
    length.y -= 1;
    
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
        int y = min(dispatchThreadID.y + gBlurRadius, length.y);
        gCache[groupThreadID.y + 2 * gBlurRadius] = gInputMap[int2(dispatchThreadID.x, y)];
    }
    
	// Clamp out of bound samples that occur at image borders.
    gCache[groupThreadID.y + gBlurRadius] = gInputMap[min(dispatchThreadID.xy, length.xy)];


	// Wait for all threads to finish.
    GroupMemoryBarrierWithGroupSync();

	//
	// Now blur each pixel.
	//

    float4 blurColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll]
    for (int k = -gBlurRadius; k <= gBlurRadius; ++k)
    {
        blurColor += gWeights[k + gBlurRadius] * gCache[groupThreadID.y + gBlurRadius + k];
    }

    gOutputUAV[dispatchThreadID.xy] = blurColor;
}