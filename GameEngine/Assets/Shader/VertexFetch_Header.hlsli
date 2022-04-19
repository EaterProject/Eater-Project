
// Bone Animation float3x4 Struct
struct BoneAnimation
{
    float4 Row1; // Matrix Row1(x,y,z), Row4(x)
    float4 Row2; // Matrix Row2(x,y,z), Row4(y)
    float4 Row3; // Matrix Row3(x,y,z), Row4(z)
};

// Bone Matrix float3x4 -> float4x4 Convert
float4x4 LoadBoneMatrix(in BoneAnimation bone)
{
    return float4x4(float4(bone.Row1.xyz, 0.0f),
                     float4(bone.Row2.xyz, 0.0f),
                     float4(bone.Row3.xyz, 0.0f),
                     float4(bone.Row1.w, bone.Row2.w, bone.Row3.w, 1.0f));
}