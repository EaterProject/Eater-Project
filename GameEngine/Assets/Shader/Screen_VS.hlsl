#include "Input_Header.hlsli"

#ifdef SSAO
cbuffer cbSsaoFrustum : register(b0)
{
	float4   gFrustumCorners[4];
};
#endif

ScreenVertexOut Screen_VS(ScreenVertexIn vin)
{
    ScreenVertexOut vout;

	//동차 공간으로 변환
	vout.PosH = float4(vin.PosL, 1.0f);

    vout.Tex = vin.Tex;
	
#ifdef SSAO
	// We store the index to the frustum corner in the normal x-coord slot.
    vout.ToFarPlane = gFrustumCorners[vin.ToFarPlaneIndex.x].xyz;
#endif
	
	return vout;
}