struct VS_OUT
{
	float4 pos : SV_Position;
};

VS_OUT VS_main(  )
{
	// 0:ar output
	VS_OUT output = (VS_OUT)0;
	output.pos = float4(1.0f, 1.0f, 1.0f, 1.0f);
	return output;
}