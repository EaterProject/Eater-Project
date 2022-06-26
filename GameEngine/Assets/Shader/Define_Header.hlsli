#ifndef DEFINE_HEADER
#define DEFINE_HEADER

#define PI          3.14159265359f
#define TWO_PI      6.28318530718f
#define PI_OVER_TWO 1.5707963268f

#define EPSILON 0.000001f

// Binding Map Type
#define ALBEDO_MAP      0x00000001
#define NORMAL_MAP      0x00000010
#define EMISSIVE_MAP    0x00000100
#define ORM_MAP         0x00001000
#define LIM_LIGHT       0x00010000
#define DISSOLVE        0x00100000

// Specular coefficiant - fixed reflectance value for non-metals
#define F_ZERO float3(0.04f, 0.04f, 0.04f)

#endif