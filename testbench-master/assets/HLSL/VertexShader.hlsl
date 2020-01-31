struct VS_OUT
{
	float4 pos : SV_Position;
};

// Detta är syntaxen som vi ej vet hur man ska skriva:
// float4 pos : register(t1)

VS_OUT VS_main(  )
{
	// 0:ar output
	VS_OUT output = (VS_OUT)0;
	output.pos = position;
	return output;
}