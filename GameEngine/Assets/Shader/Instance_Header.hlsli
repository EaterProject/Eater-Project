#define INSTANCE_SEMANTIC(SemanticName) INSTANCE_##SemanticName

struct Bone
{
    float4 Row1;
    float4 Row2;
    float4 Row3;
};

// Instance Vertex Shader Input & Output Data Struct
struct MeshDepthInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WOLRD);
};

struct MeshInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WOLRD);
    float4x4 InvWorld       : INSTANCE_SEMANTIC(INVWOLRD);

#ifdef SKIN_MESH
    uint PrevAnimationIndex     : INSTANCE_SEMANTIC(PREV_ANIMATION_INDEX);
    uint NextAnimationIndex     : INSTANCE_SEMANTIC(NEXT_ANIMATION_INDEX);
    float FrameTime             : INSTANCE_SEMANTIC(FRAME_TIME);
    float Pad                   : INSTANCE_SEMANTIC(PAD);
#endif
};

struct MeshIDInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WORLD);
    float4 HashColor        : INSTANCE_SEMANTIC(HASHCOLOR);
};

struct ParticleInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WOLRD);
    float4x4 TexTransform   : INSTANCE_SEMANTIC(TEX);
    float4 Color            : INSTANCE_SEMANTIC(COLOR);
};