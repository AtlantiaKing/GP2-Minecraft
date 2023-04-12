#pragma once
#include "Chunk.h"

class WorldRenderer final
{
public:
	~WorldRenderer();

	void LoadEffect(const SceneContext& sceneContext);
	void SetBuffers(std::vector<Chunk>& chunks, const SceneContext& sceneContext);
	void SetBuffer(Chunk& chunk, const SceneContext& sceneContext);
	void Draw(std::vector<Chunk>& chunks, const SceneContext& sceneContext);
	void Draw(Chunk& chunk, const SceneContext& sceneContext);
private:
	ID3DX11EffectMatrixVariable* m_pWorldVar{};
	ID3DX11EffectMatrixVariable* m_pWvpVar{};
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable{};

	ID3DX11Effect* m_pEffect;
	ID3DX11EffectTechnique* m_pTechnique;
	ID3D11InputLayout* m_pInputLayout;
};

