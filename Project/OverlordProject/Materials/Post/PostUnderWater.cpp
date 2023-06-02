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

	const auto pDepthSRV = pSource->GetDepthShaderResourceView();
	m_pBaseEffect->GetVariableByName("gDepthTexture")->AsShaderResource()->SetResource(pDepthSRV);

	//From this point on, pSource (RenderTarget) is bound as an SRV (ShaderResourceView, read from) to the pipeline
	//In case we want to use pSource as a RTV (RenderTargetView, render to) we have to unbind it first as an SRV
}
