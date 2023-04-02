#pragma once

class WorldRenderer final
{
public:
	~WorldRenderer();

	void LoadEffect(const SceneContext& sceneContext);
	void SetBuffer(const std::vector<VertexPosNormTex>& vertices, const SceneContext& sceneContext);
	void Draw(const SceneContext& sceneContext);
private:
	ID3DX11EffectMatrixVariable* m_pWorldVar{};
	ID3DX11EffectMatrixVariable* m_pWvpVar{};
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};

	ID3D11Buffer* m_pVertexBuffer{};
	int m_NrVertices{};

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	ID3D11InputLayout* m_pInputLayout;
};

