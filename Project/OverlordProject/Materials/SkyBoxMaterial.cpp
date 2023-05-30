#include "stdafx.h"
#include "SkyBoxMaterial.h"

SkyBoxMaterial::SkyBoxMaterial():
	Material<SkyBoxMaterial>(L"Effects/SkyBox.fx")
{
}

void SkyBoxMaterial::SetTexture(const std::wstring& assetFile)
{
	m_pTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gCubeMap", m_pTexture);
}
