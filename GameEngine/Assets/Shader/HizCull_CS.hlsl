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

// ��� �� �߽�(XYZ) �� ������(W), ���� ����
StructuredBuffer<float4> gColliderBuffer : register(t0);
// �ø� ���� 1 (�ø� X) 0 (�ø� O)
RWStructuredBuffer<float> gCullingBuffer : register(u0);

Texture2D gHizMap : register(t1);
SamplerState gSamClampPoint : register(s0);
 	 
// ���� ��� ������ ��ȣ �ִ� �Ÿ��� ����մϴ�.
// vPlane: ��� ���(a,b,c,d)�� �����մϴ�. ���⼭ ax + by + cz = d
// vPoint: ��鿡 ���� �׽�Ʈ�� ���Դϴ�.
float DistanceToPlane(float4 vPlane, float3 vPoint)
{
    return dot(float4(vPoint, 1), vPlane);
}
 	 
// ������ ����ü �ø�. ���̴� ��� > 0�� ��ȯ�ϰ� �׷��� ���� ��� <= 0�� ��ȯ�մϴ�.
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
 	// �� �׷��� �� �����尡 ���� ���� �ε����� ����մϴ�.
    int index = DispatchThreadId.x;
 	 
 	// ��� �� �߽�(XYZ) �� ������(W), ���� ����
    float4 Bounds = gColliderBuffer[index];
 	 
 	// ��-����ü �׽�Ʈ ����
    float fVisible = CullSphere(gFrustumPlanes, Bounds.xyz, Bounds.w);
 	 
    if (fVisible > 0)
    {
        //float3 viewEye = -gView._m03_m13_m23;
        float3 viewEye = gEyePos.xyz;
        float CameraSphereDistance = distance(viewEye, Bounds.xyz);
 	 
        float3 viewEyeSphereDirection = normalize(viewEye - Bounds.xyz);
 	 
        float3 viewUp = gView._m01_m11_m21;
        float3 viewRight = cross(viewEyeSphereDirection, viewUp);
        
 	    // ���� �ְ� ó���� �����ϴ�..
        float value = Bounds.w / CameraSphereDistance;
        
        // ��豸 �ȿ� �������..
        if (value >= 1.0f)
        {
            gCullingBuffer[index] = 1;
            return;
        }
        
        float tanAsin = tan(asin(value));
        float fRadius = CameraSphereDistance * tanAsin;
 	 
 	    // �� �ֺ��� ���� ���� �������� ����մϴ�.
        float3 vUpRadius = viewUp * fRadius;
        float3 vRightRadius = viewRight * fRadius;
        
 	    // ���� �������� ���� 4�� �𼭸��� �����մϴ�.
        float4 vCorner0WS = float4(Bounds.xyz + vUpRadius - vRightRadius, 1); // Top-Left
        float4 vCorner1WS = float4(Bounds.xyz + vUpRadius + vRightRadius, 1); // Top-Right
        float4 vCorner2WS = float4(Bounds.xyz - vUpRadius - vRightRadius, 1); // Bottom-Left
        float4 vCorner3WS = float4(Bounds.xyz - vUpRadius + vRightRadius, 1); // Bottom-Right
 	 
 	    // ���� 4�� �𼭸��� Ŭ�� ������ �����մϴ�.
        float4 vCorner0CS = mul(gViewProj, vCorner0WS);
        float4 vCorner1CS = mul(gViewProj, vCorner1WS);
        float4 vCorner2CS = mul(gViewProj, vCorner2WS);
        float4 vCorner3CS = mul(gViewProj, vCorner3WS);
 	 
 	    // �𼭸� ���� Ŭ�� �������� ����ȭ�� ��ġ ��ǥ�� ��ȯ�մϴ�.
        float2 vCorner0NDC = vCorner0CS.xy / vCorner0CS.w;
        float2 vCorner1NDC = vCorner1CS.xy / vCorner1CS.w;
        float2 vCorner2NDC = vCorner2CS.xy / vCorner2CS.w;
        float2 vCorner3NDC = vCorner3CS.xy / vCorner3CS.w;
        vCorner0NDC = float2(0.5, -0.5) * vCorner0NDC + float2(0.5, 0.5);
        vCorner1NDC = float2(0.5, -0.5) * vCorner1NDC + float2(0.5, 0.5);
        vCorner2NDC = float2(0.5, -0.5) * vCorner2NDC + float2(0.5, 0.5);
        vCorner3NDC = float2(0.5, -0.5) * vCorner3NDC + float2(0.5, 0.5);
 	 
 	    // ���� �ִ� 4�ؼ��� ������ �Ϸ��� ������ ����ؾ� �մϴ�.
 	    // ���簢���� ���������� �ƴ϶� ���簢���� ��ü �ʺ�,
        float fSphereWidthNDC = distance(vCorner0NDC, vCorner1NDC);
 	 
 	    // ȭ�� �������� Bounding Sphere�� �߽��� ���..
        float3 Cv = mul(gView, float4(Bounds.xyz, 1)).xyz;
 	 
 	    // ���鿡�� ī�޶� ���� ����� ���� ����ϰ� ����..
        float3 Pv = Cv - normalize(Cv) * Bounds.w;
        float4 ClosestSpherePoint = mul(gProj, float4(Pv, 1));
 	 
 	    // HiZ �ʿ��� MipLevel�� ����..
 	    // ViewPort Width, Height �� �� �� ū ���� �����մϴ�.
 	    //
 	    // �̷��� �ϸ� ��ü�� �ִ�� �����ϴ� �� ������ �����˴ϴ�.
 	    // 4���� ���ø��� ����Ʈ�� ���� ���̸� ������ 2x2 �ؼ�
        float W = fSphereWidthNDC * max(gViewportSize.x, gViewportSize.y);
        float fLOD = ceil(log2(W));
 	 
 	    // ���� �簢���� �𼭸����� ���� ������ �����ɴϴ�..
        float4 vSamples;
        vSamples.x = gHizMap.SampleLevel(gSamClampPoint, vCorner0NDC, fLOD);
        vSamples.y = gHizMap.SampleLevel(gSamClampPoint, vCorner1NDC, fLOD);
        vSamples.z = gHizMap.SampleLevel(gSamClampPoint, vCorner2NDC, fLOD);
        vSamples.w = gHizMap.SampleLevel(gSamClampPoint, vCorner3NDC, fLOD);
 	 
        float fMaxSampledDepth = max(max(vSamples.x, vSamples.y), max(vSamples.z, vSamples.w));
        float fSphereDepth = (ClosestSpherePoint.z / ClosestSpherePoint.w);
 	 
 	    // ���̰� HiZ �� �� �� ���� ū ������ ū ��� Sphere�� �ø�..
        gCullingBuffer[index] = (fSphereDepth > fMaxSampledDepth) ? 0 : 1;
    }
    else
    {
 	    // Sphere�� View-Frustum �ܺο� ���� ���..
        gCullingBuffer[index] = 0;
    }
}