#include "stdafx.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(const std::wstring& spriteAsset, const XMFLOAT2& pivot, const XMFLOAT4& color):
	m_SpriteAsset(spriteAsset),
	m_Pivot(pivot),
	m_Color(color)
{}

void SpriteComponent::Initialize(const SceneContext& /*sceneContext*/)
{
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::SetTexture(const std::wstring& spriteAsset)
{
	m_SpriteAsset = spriteAsset;
	m_pTexture = ContentManager::Load<TextureData>(m_SpriteAsset);
}

void SpriteComponent::Draw(const SceneContext& sceneContext)
{
	if (!m_pTexture)
		return;

	TransformComponent* pTransform{ GetGameObject()->GetComponent<TransformComponent>() };
	const XMFLOAT3& position{ pTransform->GetWorldPosition() };
	const XMFLOAT3& scale{ pTransform->GetWorldScale() };

	SpriteRenderer::Get()->DrawImmediate(
		sceneContext.d3dContext, m_pTexture->GetShaderResourceView(),
		XMFLOAT2{ position.x, position.y }, m_Color,
		m_Pivot, XMFLOAT2{ scale.x, scale.y }, 
		MathHelper::QuaternionToEuler(pTransform->GetWorldRotation()).z,
		pTransform->GetPosition().z);
}
