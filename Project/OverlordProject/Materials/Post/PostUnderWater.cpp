#include "stdafx.h"
#include "PostUnderWater.h"

PostUnderWater::PostUnderWater()
	: PostProcessingMaterial(L"Effects/Post/Underwater.fx")
{
}

void PostUnderWater::UpdateBaseEffectVariables(const SceneContext&, RenderTarget* pSource)
{
	//Set Source Texture
	const auto pSourceSRV = pSource->GetColorShaderResourceView();
	m_pBaseEffect->GetVariableByName("gTexture")->AsShaderResource()->SetResource(pSourceSRV);

	//Set Depth Texture
	const auto pDepthSRV = pSource->GetDepthShaderResourceView();
	m_pBaseEffect->GetVariableByName("gDepthTexture")->AsShaderResource()->SetResource(pDepthSRV);
}