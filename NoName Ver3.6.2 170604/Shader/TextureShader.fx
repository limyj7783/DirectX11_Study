
Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);


cbuffer cbView : register(b0)
{
	matrix View;
}

cbuffer cbProjection : register(b1)
{
	matrix Projection;
}

cbuffer cbWorld : register(b2)
{
	matrix World;
	float4 vMeshColor;
}

struct TVS_INPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

struct TPS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD0;
};

// Vertex Shader
TPS_INPUT TextureVertexShader(TVS_INPUT input)
{
	TPS_INPUT output = (TPS_INPUT)0;
	output.Pos = mul(input.Pos, World);
	output.Pos = mul(output.Pos, View);
	output.Pos = mul(output.Pos, Projection);
	output.Tex = input.Tex;

	return output;
}

// Pixel Shader
float4 TexturePixelShader( TPS_INPUT input ) : SV_Target
{
	return txDiffuse.Sample(samLinear, input.Tex) * vMeshColor;
}