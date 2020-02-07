struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : UV;
};


cbuffer color : register(b0)
{
	float r, g, b, a;
}

Texture2D tex : register (t0);
SamplerState samLinear : register (s0);

float4 PS_main(VS_OUT input) : SV_TARGET0
{
	#ifdef DIFFUSE_SLOT
		return tex.Sample(samLinear, input.uv);
	#else
		return float4(r,g,b,a);
	#endif
}