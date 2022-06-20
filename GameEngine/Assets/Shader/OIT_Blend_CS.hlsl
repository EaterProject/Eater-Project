#include "Output_Header.hlsli"
#include "OIT_Header.hlsli"

StructuredBuffer<FLStaticNode> gPieceLinkBuffer : register(t0);
ByteAddressBuffer gFirstOffsetBuffer : register(t1);

Texture2D gBackGround : register(t2);
RWTexture2D<float4> gOutputUAV : register(u0);

static FragmentData gSortedPixels[MAX_SORTED_PIXELS];

[numthreads(1, 1, 1)]
void OIT_Blend_CS(uint3 groupThreadID : SV_GroupThreadID, uint3 dispatchID : SV_DispatchThreadID)
{
    int startOffsetAddress = 4 * (gFrameWidth * dispatchID.y + dispatchID.x);
    int numPixels = 0;
    uint offset = gFirstOffsetBuffer.Load(startOffsetAddress);
    
    FLStaticNode element;
    
    while (offset != 0xFFFFFFFF)
    {
        element = gPieceLinkBuffer[offset];
        gSortedPixels[numPixels++] = element.Data;
        offset = (numPixels >= MAX_SORTED_PIXELS) ?
            0xFFFFFFFF : element.Next;
    }
    
    FragmentData temp;
    
    for (int i = 1; i < numPixels; ++i)
    {
        for (int j = i - 1; j >= 0; --j)
        {
            if (gSortedPixels[j].Depth < gSortedPixels[j + 1].Depth)
            {
                temp = gSortedPixels[j];
                gSortedPixels[j] = gSortedPixels[j + 1];
                gSortedPixels[j + 1] = temp;
            }
            else
            {
                break;
            }
        }
    }

    float4 currColor = gBackGround.Load(int3(dispatchID.xy, 0));

    FragmentData data;
    
	[unroll]
    for (int l = 0; l < numPixels; ++l)
    {
        data = gSortedPixels[l];
        float4 pixelColor = UnpackColorFromUInt(data.Color);
        currColor.xyz = lerp(currColor.xyz, pixelColor.xyz * data.Strength, pixelColor.w);
    }
    
    gOutputUAV[dispatchID.xy] = currColor;
}
