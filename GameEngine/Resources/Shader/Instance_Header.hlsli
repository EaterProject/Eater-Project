#define INSTANCE_SEMANTIC(SemanticName) INSTANCE_##SemanticName

// Instance Vertex Shader Input & Output Data Struct
struct MeshInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WOLRD);
    float4x4 InvWorld       : INSTANCE_SEMANTIC(INVWOLRD);
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