struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};

cbuffer cbTextColor : register(b0)
{
    float4 textColor;
};

Texture2D<float> fontTextureAltas : register(t0);
SamplerState linearSamplerState : register(s0);

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    float textAlpha = fontTextureAltas.Sample(linearSamplerState, input.uv);
    
    output.color = float4(textColor.r, textColor.g, textColor.b, textAlpha);
    
    return output;
}