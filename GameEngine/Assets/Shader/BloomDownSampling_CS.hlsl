Texture2D<float4> gInputMap : register(t0);
Texture2D<float4> gPositionRT : register(t1);
RWTexture2D<float4> gOutputUAV : register(u0);

cbuffer cbBloomDownSampling : register(b0)
{
    float3 gEyePosW;
    float gThreshold;
}

[numthreads(8, 8, 1)]
void BloomDownSampling_CS(uint3 groupID : SV_GroupID, uint3 groupThreadID : SV_GroupThreadID, uint groupIndex : SV_GroupIndex, uint3 dispatchID : SV_DispatchThreadID)
{
    // output pixel in half resolution
    uint2 pixel = uint2(dispatchID.x, dispatchID.y);

    // bilinear interpolation for downsampling
    uint2 inPixel = pixel * 2;
    float4 hIntensity0 = lerp(gInputMap[inPixel], gInputMap[inPixel + uint2(1, 0)], 0.5);
    float4 hIntensity1 = lerp(gInputMap[inPixel + uint2(0, 1)], gInputMap[inPixel + uint2(1, 1)], 0.5);
    float4 intensity = lerp(hIntensity0, hIntensity1, 0.5) * 7.0f;

    float distance = 1.0f - abs(length(gEyePosW - gPositionRT[inPixel].xyz) * 0.0025f);

    
    
    // thresholding on downsampled value
    //float intensityTest = (float) (length(intensity.rgb) > gThreshold);
        
    if (distance <= 0.0f)
        gOutputUAV[pixel] = float4(0.0f, 0.0f, 0.0f, 1.0);
    else
        gOutputUAV[pixel] = float4(intensity.rgb, 1.0);
        
    //gOutputUAV[pixel] = float4(intensity.rgb * 2.0f, 1.0);
}