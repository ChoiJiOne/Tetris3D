struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct PS_OUTPUT
{
    float4 color : SV_TARGET0;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;
    
    output.color = input.color;
    
    return output;
}