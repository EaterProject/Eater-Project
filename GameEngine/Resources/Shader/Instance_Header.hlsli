#define INSTANCE_SEMANTIC(SemanticName) INSTANCE_##SemanticName

// Instance Vertex Shader Input & Output Data Struct
struct MeshInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WOLRD);
};

struct ParticleInstanceIn
{
    float4x4 World          : INSTANCE_SEMANTIC(WOLRD);
    float4x4 TexTransform   : INSTANCE_SEMANTIC(TEX);
    float4 Color            : INSTANCE_SEMANTIC(COLOR);
};