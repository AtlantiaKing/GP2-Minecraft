#pragma once

#include "Misc/World/WorldData.h"

class BlockRenderer final : public BaseComponent
{
public:
	BlockRenderer(BlockType type, const SceneContext& sceneContext);
	virtual ~BlockRenderer();

	BlockRenderer(const BlockRenderer& other) = delete;
	BlockRenderer(BlockRenderer&& other) noexcept = delete;
	BlockRenderer& operator=(const BlockRenderer& other) = delete;
	BlockRenderer& operator=(BlockRenderer&& other) noexcept = delete;

protected:
	virtual void Initialize(const SceneContext&) override {}
	virtual void Draw(const SceneContext& sceneContext) override;

private:
	ID3DX11EffectMatrixVariable* m_pWorldVar{};
	ID3DX11EffectMatrixVariable* m_pWvpVar{};

	ID3D11Buffer* m_pVertexBuffer{};
	int m_NrVertices{};

	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3D11InputLayout* m_pInputLayout{};
};

