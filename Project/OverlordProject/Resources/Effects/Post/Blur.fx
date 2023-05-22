//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;

// Create Depth Stencil State (ENABLE DEPTH WRITING)
DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

BlendState DisableBlending
{
    BlendEnable[0] = FALSE;
};

/// Create Rasterizer State (Backface culling) 
RasterizerState BackCulling
{
	CullMode = BACK;
};

//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
	float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float2 TexCoord : TEXCOORD1;
};


int gIterations = 5;


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Position = float4(input.Position, 1.0f);
	output.TexCoord = input.TexCoord;
	
	return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input): SV_Target
{
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	// Step 1: find the dimensions of the texture (the texture has a method for that)	
	uint2 dimensions;
	gTexture.GetDimensions(dimensions.x, dimensions.y);
	// Step 2: calculate dx and dy (UV space for 1 pixel)	
	float dx = 1.0f / dimensions.x;
	float dy = 1.0f / dimensions.y;
	// Step 3: Create a double for loop (5 iterations each)
	//		   Inside the loop, calculate the offset in each direction. Make sure not to take every pixel but move by 2 pixels each time
	//			Do a texture lookup using your previously calculated uv coordinates + the offset, and add to the final color
	for(int x = -gIterations / 2; x <= gIterations / 2; ++x)
	{
		for(int y = -gIterations / 2; y <= gIterations / 2; ++y)
		{
			finalColor += gTexture.Sample(samPoint, input.TexCoord + float2(x * dx * 2, y * dy * 2));
		}
	}
	// Step 4: Divide the final color by the number of passes (in this case 5*5)	
	finalColor /= gIterations * gIterations;
	// Step 5: return the final color
	return finalColor;
}


//TECHNIQUE
//---------
technique11 Blur
{
    pass P0
    {
		SetRasterizerState(BackCulling);
        SetDepthStencilState(EnableDepth, 0);
        SetBlendState(DisableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}