struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};

cbuffer cbTransparent : register(b0)
{
    float4 transparent;
}

Texture2D<float4> textureResource : register(t0);
SamplerState linearSamplerState : register(s0);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    output.color = textureResource.Sample(linearSamplerState, input.uv);
    output.color.a = transparent.a;
    
    return output;
}