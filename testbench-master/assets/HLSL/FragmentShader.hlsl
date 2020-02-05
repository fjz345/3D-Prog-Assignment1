struct VS_OUT
{
	float4 pos : SV_Position;
};

cbuffer color : register(b0)
{
	float r, g, b, a;
}

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	return float4(r,g,b,a);
}