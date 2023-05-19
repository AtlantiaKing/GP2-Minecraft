#include "stdafx.h"
#include "DiffuseMaterial_Shadow_Skinned.h"

DiffuseMaterial_Shadow_Skinned::DiffuseMaterial_Shadow_Skinned():
	Material(L"Effects/Shadow/PosNormTex3D_Shadow_Skinned.fx")
{}

void DiffuseMaterial_Shadow_Skinned::SetDiffuseTexture(const std::wstring& assetFile)
{
	m_pDiffuseTexture = ContentManager::Load<TextureData>(assetFile);
	SetVariable_Texture(L"gDiffuseMap", m_pDiffuseTexture);
}

void DiffuseMaterial_Shadow_Skinned::InitializeEffectVariables()
{
}

void DiffuseMaterial_Shadow_Skinned::OnUpdateModelVariables(const SceneContext& sceneContext, const ModelComponent* pModel) const
{
	/*
	 * TODO_W8
	 * Update The Shader Variables
	 * 1. Update the LightWVP > Used to Transform a vertex into Light clipping space
	 * 	LightWVP = model_world * light_viewprojection
	 * 	(light_viewprojection [LightVP] can be acquired from the ShadowMapRenderer)
	*/
	const XMMATRIX lightWVP{ XMLoadFloat4x4(&pModel->GetTransform()->GetWorld()) *
							 XMLoadFloat4x4(&ShadowMapRenderer::Get()->GetLightVP()) };
	XMFLOAT4X4 lightWVPFloat;
	XMStoreFloat4x4(&lightWVPFloat, lightWVP);

	SetVariable_Matrix(L"gWorldViewProj_Light", reinterpret_cast<const float*>(&lightWVPFloat));

	// 2. Update the ShadowMap texture
	SetVariable_Texture(L"gShadowMap", ShadowMapRenderer::Get()->GetShadowMap());

	// 3. Update the Light Direction (retrieve the direction from the LightManager > sceneContext)
	SetVariable_Vector(L"gLightDirection", sceneContext.pLights->GetDirectionalLight().direction);

	//Retrieve The Animator from the ModelComponent
	ModelAnimator* pAnimator{ pModel->GetAnimator() };

	//Make sure the animator is not NULL (ASSERT_NULL_)
	ASSERT_NULL_(pAnimator);

	//Retrieve the BoneTransforms from the Animator
	const std::vector<XMFLOAT4X4>& boneTransforms{ pAnimator->GetBoneTransforms() };

	//Set the 'gBones' variable of the effect (MatrixArray) > BoneTransforms
	SetVariable_MatrixArray(L"gBones", reinterpret_cast<const float*>(boneTransforms.data()), static_cast<UINT>(boneTransforms.size()));
}
