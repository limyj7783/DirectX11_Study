

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_OUTPUT
{
	float4 Pos		: SV_POSITION;
	float4 Color	: COLOR0;
};

VS_OUTPUT ColorVertexShader(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.Pos = mul(Pos, World);
	Output.Pos = mul(Output.Pos, View);
	Output.Pos = mul(Output.Pos, Projection);
	Output.Color = Color;

	return Output;
}

float4 ColorPixelShader( VS_OUTPUT Input ) : SV_Target
{
	return Input.Color;
}