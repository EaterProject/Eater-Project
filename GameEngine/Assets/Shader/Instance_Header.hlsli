#define INSTANCE_SEMANTIC(SemanticName) INSTANCE_##SemanticName

// Instance Vertex Shader Input & Output Data Struct
struct MeshDepthInstanceIn
{
    float4x4 World              : INSTANCE_SEMANTIC(WOLRD);
    
#ifdef SKIN_MESH
    uint PrevAnimationIndex     : INSTANCE_SEMANTIC(PREV);
    uint NextAnimationIndex     : INSTANCE_SEMANTIC(NEXT);
    float FrameTime             : INSTANCE_SEMANTIC(TIME);
    float Pad                   : INSTANCE_SEMANTIC(PAD);
#endif
};

struct MeshInstanceIn
{
    float4x4 World              : INSTANCE_SEMANTIC(WOLRD);
    float4x4 InvWorld           : INSTANCE_SEMANTIC(INVWOLRD);
    
#ifdef SKIN_MESH
    uint PrevAnimationIndex     : INSTANCE_SEMANTIC(PREV);
    uint NextAnimationIndex     : INSTANCE_SEMANTIC(NEXT);
    float FrameTime             : INSTANCE_SEMANTIC(TIME);
    float Pad                   : INSTANCE_SEMANTIC(PAD);
#endif
};

struct MeshIDInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WORLD);
    float4 HashColor        : INSTANCE_SEMANTIC(HASHCOLOR);
    
#ifdef SKIN_MESH
    uint PrevAnimationIndex     : INSTANCE_SEMANTIC(PREV);
    uint NextAnimationIndex     : INSTANCE_SEMANTIC(NEXT);
    float FrameTime             : INSTANCE_SEMANTIC(TIME);
    float Pad                   : INSTANCE_SEMANTIC(PAD);
#endif
};

struct ParticleInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WOLRD);
    float2 TexScale         : INSTANCE_SEMANTIC(SCALE);
    float2 TexPos           : INSTANCE_SEMANTIC(POS);
    float4 Color            : INSTANCE_SEMANTIC(COLOR);
};