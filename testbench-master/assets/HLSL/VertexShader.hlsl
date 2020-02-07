struct VS_OUT
{
	float4 pos : SV_Position;
	float2 uv : UV;
};

StructuredBuffer<float4> vbPos : register(t1);
StructuredBuffer<float4> vbNor : register(t2);
StructuredBuffer<float2> vbUV : register(t3);

cbuffer trans : register(b1)
{
	float x, y, z, w;
}

VS_OUT VS_main( uint vID : SV_VertexID )
{
	// 0:ar output
	VS_OUT output = (VS_OUT)0;
	float4 position = float4(vbPos[vID].xyz,1.0);

	output.pos = position + float4(x,y,-z,w);
	output.uv = vbUV[vID];
	return output;
}