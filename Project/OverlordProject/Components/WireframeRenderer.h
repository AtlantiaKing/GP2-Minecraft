#pragma once

class WireframeRenderer final : public BaseComponent
{
public:
	WireframeRenderer(const SceneContext& sceneContext);
	virtual ~WireframeRenderer() = default;

	WireframeRenderer(const WireframeRenderer& other) = delete;
	WireframeRenderer(WireframeRenderer&& other) noexcept = delete;
	WireframeRenderer& operator=(const WireframeRenderer& other) = delete;
	WireframeRenderer& operator=(WireframeRenderer&& other) noexcept = delete;

protected:
	virtual void Initialize(const SceneContext& sceneContext) override;
	virtual void Draw(const SceneContext& sceneContext) override;

private:
	ID3DX11EffectMatrixVariable* m_pWvpVar{};

	ID3D11Buffer* m_pVertexBuffer{};
	int m_NrVertices{};

	ID3DX11Effect* m_pEffect{};
	ID3DX11EffectTechnique* m_pTechnique{};
	ID3D11InputLayout* m_pInputLayout{};
};

