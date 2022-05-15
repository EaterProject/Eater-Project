cbuffer cbHizCull : register(b0)
{
    matrix gView;
    matrix gProj;
    matrix gViewProj;
 	 
    float4 gFrustumPlanes[6];
    float4 gEyePos;
    
    float2 gViewportSize;
 	 
    float2 gPad;
};

// 경계 구 중심(XYZ) 및 반지름(W), 세계 공간
StructuredBuffer<float4> gColliderBuffer : register(t0);
// 컬링 여부 1 (컬링 X) 0 (컬링 O)
RWStructuredBuffer<float> gCullingBuffer : register(u0);

Texture2D gHizMap : register(t1);
SamplerState gSamClampPoint : register(s0);
 	 
// 점과 평면 사이의 부호 있는 거리를 계산합니다.
// vPlane: 평면 계수(a,b,c,d)를 포함합니다. 여기서 ax + by + cz = d
// vPoint: 평면에 대해 테스트할 점입니다.
float DistanceToPlane(float4 vPlane, float3 vPoint)
{
    return dot(float4(vPoint, 1), vPlane);
}
 	 
// 구에서 절두체 컬링. 보이는 경우 > 0을 반환하고 그렇지 않은 경우 <= 0을 반환합니다.
float CullSphere(float4 vPlanes[6], float3 vCenter, float fRadius)
{
    float dist01 = min(DistanceToPlane(vPlanes[0], vCenter), DistanceToPlane(vPlanes[1], vCenter));
    float dist23 = min(DistanceToPlane(vPlanes[2], vCenter), DistanceToPlane(vPlanes[3], vCenter));
    float dist45 = min(DistanceToPlane(vPlanes[4], vCenter), DistanceToPlane(vPlanes[5], vCenter));
 	 
    return min(min(dist01, dist23), dist45) + fRadius;
}
 	 
[numthreads(1, 1, 1)]
 	void HizCull_CS(uint3 GroupId : SV_GroupID,
 	uint3 DispatchThreadId : SV_DispatchThreadID,
 	uint GroupIndex : SV_GroupIndex)
{
 	// 이 그룹의 이 스레드가 읽을 실제 인덱스를 계산합니다.
    int index = DispatchThreadId.x;
 	 
 	// 경계 구 중심(XYZ) 및 반지름(W), 세계 공간
    float4 Bounds = gColliderBuffer[index];
 	 
 	// 뷰-절두체 테스트 수행
    float fVisible = CullSphere(gFrustumPlanes, Bounds.xyz, Bounds.w);
 	 
    if (fVisible > 0)
    {
        //float3 viewEye = -gView._m03_m13_m23;
        float3 viewEye = gEyePos.xyz;
        float CameraSphereDistance = distance(viewEye, Bounds.xyz);
 	 
        float3 viewEyeSphereDirection = normalize(viewEye - Bounds.xyz);
 	 
        float3 viewUp = gView._m01_m11_m21;
        float3 viewRight = cross(viewEyeSphereDirection, viewUp);
        
 	    // 원근 왜곡 처리를 돕습니다..
        float value = Bounds.w / CameraSphereDistance;
        
        // 경계구 안에 있을경우..
        if (value >= 1.0f)
        {
            gCullingBuffer[index] = 1;
            return;
        }
        
        float tanAsin = tan(asin(value));
        float fRadius = CameraSphereDistance * tanAsin;
 	 
 	    // 구 주변의 점에 대한 오프셋을 계산합니다.
        float3 vUpRadius = viewUp * fRadius;
        float3 vRightRadius = viewRight * fRadius;
        
 	    // 세계 공간에서 구의 4개 모서리를 생성합니다.
        float4 vCorner0WS = float4(Bounds.xyz + vUpRadius - vRightRadius, 1); // Top-Left
        float4 vCorner1WS = float4(Bounds.xyz + vUpRadius + vRightRadius, 1); // Top-Right
        float4 vCorner2WS = float4(Bounds.xyz - vUpRadius - vRightRadius, 1); // Bottom-Left
        float4 vCorner3WS = float4(Bounds.xyz - vUpRadius + vRightRadius, 1); // Bottom-Right
 	 
 	    // 구의 4개 모서리를 클립 공간에 투영합니다.
        float4 vCorner0CS = mul(gViewProj, vCorner0WS);
        float4 vCorner1CS = mul(gViewProj, vCorner1WS);
        float4 vCorner2CS = mul(gViewProj, vCorner2WS);
        float4 vCorner3CS = mul(gViewProj, vCorner3WS);
 	 
 	    // 모서리 점을 클립 공간에서 정규화된 장치 좌표로 변환합니다.
        float2 vCorner0NDC = vCorner0CS.xy / vCorner0CS.w;
        float2 vCorner1NDC = vCorner1CS.xy / vCorner1CS.w;
        float2 vCorner2NDC = vCorner2CS.xy / vCorner2CS.w;
        float2 vCorner3NDC = vCorner3CS.xy / vCorner3CS.w;
        vCorner0NDC = float2(0.5, -0.5) * vCorner0NDC + float2(0.5, 0.5);
        vCorner1NDC = float2(0.5, -0.5) * vCorner1NDC + float2(0.5, 0.5);
        vCorner2NDC = float2(0.5, -0.5) * vCorner2NDC + float2(0.5, 0.5);
        vCorner3NDC = float2(0.5, -0.5) * vCorner3NDC + float2(0.5, 0.5);
 	 
 	    // 구가 최대 4텍셀을 덮도록 하려면 다음을 사용해야 합니다.
 	    // 직사각형의 반지름만이 아니라 직사각형의 전체 너비,
        float fSphereWidthNDC = distance(vCorner0NDC, vCorner1NDC);
 	 
 	    // 화면 공간에서 Bounding Sphere의 중심을 계산..
        float3 Cv = mul(gView, float4(Bounds.xyz, 1)).xyz;
 	 
 	    // 구면에서 카메라에 가장 가까운 점을 계산하고 투영..
        float3 Pv = Cv - normalize(Cv) * Bounds.w;
        float4 ClosestSpherePoint = mul(gProj, float4(Pv, 1));
 	 
 	    // HiZ 맵에서 MipLevel을 선택..
 	    // ViewPort Width, Height 둘 중 더 큰 값을 결정합니다.
 	    //
 	    // 이렇게 하면 객체가 최대로 차지하는 밉 레벨이 생성됩니다.
 	    // 4개의 샘플링된 포인트가 비교할 깊이를 갖도록 2x2 텍셀
        float W = fSphereWidthNDC * max(gViewportSize.x, gViewportSize.y);
        float fLOD = ceil(log2(W));
 	 
 	    // 비교할 사각형의 모서리에서 깊이 샘플을 가져옵니다..
        float4 vSamples;
        vSamples.x = gHizMap.SampleLevel(gSamClampPoint, vCorner0NDC, fLOD);
        vSamples.y = gHizMap.SampleLevel(gSamClampPoint, vCorner1NDC, fLOD);
        vSamples.z = gHizMap.SampleLevel(gSamClampPoint, vCorner2NDC, fLOD);
        vSamples.w = gHizMap.SampleLevel(gSamClampPoint, vCorner3NDC, fLOD);
 	 
        float fMaxSampledDepth = max(max(vSamples.x, vSamples.y), max(vSamples.z, vSamples.w));
        float fSphereDepth = (ClosestSpherePoint.z / ClosestSpherePoint.w);
 	 
 	    // 깊이가 HiZ 맵 값 중 가장 큰 값보다 큰 경우 Sphere를 컬링..
        gCullingBuffer[index] = (fSphereDepth > fMaxSampledDepth) ? 0 : 1;
    }
    else
    {
 	    // Sphere가 View-Frustum 외부에 있을 경우..
        gCullingBuffer[index] = 0;
    }
}