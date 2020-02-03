struct VS_OUT
{
	float4 pos : SV_Position;
};

StructuredBuffer<float3> vbPos : register(t1);
StructuredBuffer<float3> vbNor : register(t2);
StructuredBuffer<float2> vbuv : register(t3);

VS_OUT VS_main( uint vID : SV_VertexID )
{
	// 0:ar output
	VS_OUT output = (VS_OUT)0;
	float4 position = float4(vbPos[vID], 1.0);
	output.pos = position;
	return output;
}