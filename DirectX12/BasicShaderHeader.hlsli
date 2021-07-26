cbuffer cbuff0 : register(b0)
{
    float4 color;
    matrix mat;
    matrix world;
    float3 cameraPos;
};

cbuffer cbuff1 : register(b1)
{
    float3 m_ambient : packoffset(c0);
    float3 m_diffuse : packoffset(c1);
    float3 m_specular : packoffset(c2);
    float m_alpha : packoffset(c2.w);
};

static const int DIRECTIONAL_LIGHT_NUM = 1;
static const int POINT_LIGHT_NUM = 3;

struct DirLight
{
    float3 direction;
    float3 lightColor;
    uint isActive;
};

struct PointLight
{
    float3 lightPos;
    float3 lightColor;
    float3 lightAtten;
    uint isActive;
};

cbuffer cbuff2 : register(b2)
{
    matrix lightCamera;
    matrix shadow;
    float3 eye;
    float3 ambientColor;
    DirLight dirLights[DIRECTIONAL_LIGHT_NUM];
    PointLight pointLights[POINT_LIGHT_NUM];
};

struct VSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 worldpos : POSITION;
    float4 tpos : TPOS;
};

struct GSOutput
{
    float4 svpos : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 worldpos : POSITION;
    float4 tpos : TPOS;
};