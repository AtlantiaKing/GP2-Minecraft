#pragma once

class BlockBreakRenderer final : public BaseComponent
{
public:
	BlockBreakRenderer(const SceneContext& sceneContext);
	virtual ~BlockBreakRenderer();

	BlockBreakRenderer(const BlockBreakRenderer& other) = delete;
	BlockBreakRenderer(BlockBreakRenderer&& other) noexcept = delete;
	BlockBreakRenderer& operator=(const BlockBreakRenderer& other) = delete;
	BlockBreakRenderer& operator=(BlockBreakRenderer&& other) noexcept = delete;

	void SetBreakingStage(int stage) const;

	void SetVisibility(bool visible) { m_CanRender = visible; }
	bool IsVisible() const { return m_CanRender; }

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Draw(const SceneContext& sceneContext) override;

private:
	bool m_CanRender{};

	ID3DX11EffectMatrixVariable* m_pWvpVar{};
	ID3DX11EffectScalarVariable* m_pStageVar{};
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};

	ID3D11Buffer* m_pVertexBuffer{};
	int m_NrVertices{};

	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3D11InputLayout* m_pInputLayout{};
};

