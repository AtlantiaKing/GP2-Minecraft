float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
float gLightIntensity = 2.0f;

struct VS_INPUT
{
	float3 pos : POSITION;
};
struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState Wireframe
{
	CullMode = BACK;
	FillMode = WIREFRAME;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input){

	VS_OUTPUT output;
	output.pos = mul(float4(input.pos,1.0f), gWorldViewProj);
	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(Wireframe);
		SetDepthStencilState(EnableDepth, 0);

		SetVertexShader( CompileShader( vs_4_0, VS() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}

