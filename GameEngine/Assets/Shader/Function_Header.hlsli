#include "Define_Header.hlsli"

float3 UnpackNormal(in float3 normalMapSample, in float3 normal, in float3 tangent)
{
    normal = normalize(normal);
    tangent = normalize(tangent);
    
	// Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

	// Build orthonormal basis.
    float3 N = normal;
    float3 T = normalize(tangent - dot(tangent, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

	// Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}

half3 GammaToLinearSpace (half3 sRGB)
{
    // Approximate version from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return sRGB * (sRGB * (sRGB * 0.305306011h + 0.682171111h) + 0.012522878h);
}

half3 LinearToGammaSpace(half3 linRGB)
{
    linRGB = max(linRGB, half3(0.h, 0.h, 0.h));
    
    // An almost-perfect approximation from http://chilliant.blogspot.com.au/2012/08/srgb-approximations-for-hlsl.html?m=1
    return max(1.055h * pow(linRGB, 0.416666667h) - 0.055h, 0.h);
}

float2 DirectionToEquirectUV(float3 v)
{
	// https://msdn.microsoft.com/en-us/library/windows/desktop/bb509575(v=vs.85).aspx
	// The signs of the x and y parameters are used to determine the quadrant of the return values 
	// within the range of -PI to PI. The atan2 HLSL intrinsic function is well-defined for every point 
	// other than the origin, even if y equals 0 and x does not equal 0.
    float2 uv = float2(atan2(v.z, v.x), asin(-v.y));
    uv /= float2(-TWO_PI, PI);
    uv += float2(0.5, 0.5);
    return uv;
}

float4 FilmicToneMapping(float4 colorIn)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    float W = 11.2f;

    float exposureBias = 2.0f;

    float3 x = exposureBias * colorIn.rgb;

    float3 curr = ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;

    //float3 whiteScale = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    float3 whiteScale = 0.3f;
    float3 color = curr / whiteScale;

    return float4(color, colorIn.w);
}

float4 FilmicToneMapping(float3 colorIn)
{
    float A = 0.15f;
    float B = 0.50f;
    float C = 0.10f;
    float D = 0.20f;
    float E = 0.02f;
    float F = 0.30f;
    float W = 11.2f;

    float exposureBias = 2.0f;

    float3 x = exposureBias * colorIn;

    float3 curr = ((x * (A * x + C * B) + D * E) / (x * (A * x + B) + D * F)) - E / F;

    //float3 whiteScale = ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
    float3 whiteScale = 0.3f;
    float3 color = curr / whiteScale;

    return float4(color, 1.0f);
}

float3 Multiply(const float3 m[3], const float3 v)
{
    float x = m[0][0] * v[0] + m[0][1] * v[1] + m[0][2] * v[2];
    float y = m[1][0] * v[1] + m[1][1] * v[1] + m[1][2] * v[2];
    float z = m[2][0] * v[1] + m[2][1] * v[1] + m[2][2] * v[2];
    return float3(x, y, z);
}

float3 rtt_and_odt_fit(float3 v)
{
    float3 a = v * (v + 0.0245786f) - 0.000090537f;
    float3 b = v * (0.983729f * v + 0.4329510f) + 0.238081f;
    return a / b;
}

// sRGB => XYZ => D65_2_D60 => AP1 => RRT_SAT
static float3 aces_input_matrix[3] = { float3(0.59719f, 0.35458f, 0.04823f), float3(0.07600f, 0.90834f, 0.01566f), float3(0.02840f, 0.13383f, 0.83777f) };
// ODT_SAT => XYZ => D60_2_D65 => sRGB
static float3 aces_output_matrix[3] = { float3(1.60475f, -0.53108f, -0.07367f), float3(-0.10208f, 1.10813f, -0.00605f), float3(-0.00327f, -0.07276f, 1.07602f) };

float3 ACESToneMapping(float3 colorIn)
{
    colorIn = Multiply(aces_input_matrix, colorIn);
    colorIn = rtt_and_odt_fit(colorIn);
    colorIn = Multiply(aces_output_matrix, colorIn);
    
    return saturate(colorIn);
}

float4 ACESToneMapping(float4 colorIn)
{
    colorIn.rgb = Multiply(aces_input_matrix, colorIn.rgb);
    colorIn.rgb = rtt_and_odt_fit(colorIn.rgb);
    colorIn.rgb = Multiply(aces_output_matrix, colorIn.rgb);
    
    return float4(saturate(colorIn.rgb), colorIn.w);
}

float3 ACESFilm(float3 x)
{
    float a = 2.51f;
    float b = 0.03f;
    float c = 2.43f;
    float d = 0.59f;
    float e = 0.14f;
    return saturate((x * (a * x + b)) / (x * (c * x + d) + e));
}

float3 ScreenPosToWorldPos(float2 ScreenSpaceUV, float Depth, float4x4 ViewProjInv)
{
    float4 PosClipSpace;
    PosClipSpace.xy = ScreenSpaceUV * float2(2.0, -2.0) + float2(-1.0, 1.0);
    PosClipSpace.z = Depth;
    PosClipSpace.w = 1.0;
    float4 WorldPos = mul(PosClipSpace, ViewProjInv);
    return WorldPos.xyz / WorldPos.w;
}
