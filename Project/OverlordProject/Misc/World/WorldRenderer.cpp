#include "stdafx.h"
#include "WorldRenderer.h"

void WorldRenderer::SetBuffers(std::vector<Chunk>& chunks, const SceneContext& sceneContext)
{
	for (Chunk& chunk : chunks)
	{
		if(chunk.verticesChanged) SetBuffer(chunk, sceneContext);
	}
}

void WorldRenderer::SetBuffer(Chunk& chunk, const SceneContext& sceneContext)
{
	const auto& vertices{ chunk.vertices };

	if (vertices.size() == 0) return;

	//*************
	//VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBuffDesc{};
	vertexBuffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.ByteWidth = static_cast<UINT>(sizeof(VertexPosNormTex) * vertices.size());
	vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	vertexBuffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.MiscFlags = 0;

	if (chunk.pVertexBuffer) SafeRelease(chunk.pVertexBuffer);

	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices.data();

	chunk.vertexBufferSize = static_cast<int>(chunk.vertices.size());
	sceneContext.d3dContext.pDevice->CreateBuffer(&vertexBuffDesc, &initData, &chunk.pVertexBuffer);
}

WorldRenderer::~WorldRenderer()
{
	SafeRelease(m_pInputLayout);
}

void WorldRenderer::LoadEffect(const SceneContext& sceneContext)
{
	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Effects\\World.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	EffectHelper::BuildInputLayout(sceneContext.d3dContext.pDevice, m_pTechnique, &m_pInputLayout);

	if (!m_pWorldVar)
		m_pWorldVar = m_pEffect->GetVariableBySemantic("World")->AsMatrix();

	if (!m_pWvpVar)
		m_pWvpVar = m_pEffect->GetVariableBySemantic("WorldViewProjection")->AsMatrix();

	if(!m_pDiffuseMapVariable)
		m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();

	m_pDiffuseMapVariable->SetResource(ContentManager::Load<TextureData>(L"Textures\\TileAtlas.png")->GetShaderResourceView());
}

void WorldRenderer::Draw(std::vector<Chunk>& chunks, const SceneContext& sceneContext)
{
	const D3D11Context& deviceContext{ sceneContext.d3dContext };

	constexpr XMFLOAT4X4 worldMatrix{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);
	const auto viewProjection = XMLoadFloat4x4(&sceneContext.pCamera->GetViewProjection());

	m_pWorldVar->SetMatrix(reinterpret_cast<float*>(&world));
	m_pWvpVar->SetMatrix(reinterpret_cast<const float*>(&viewProjection));

	deviceContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	constexpr UINT offset = 0;
	constexpr UINT stride = sizeof(VertexPosNormTex);

	for (const Chunk& chunk : chunks)
	{
		deviceContext.pDeviceContext->IASetVertexBuffers(0, 1, &chunk.pVertexBuffer, &stride, &offset);

		D3DX11_TECHNIQUE_DESC techDesc{};
		m_pTechnique->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			m_pTechnique->GetPassByIndex(p)->Apply(0, deviceContext.pDeviceContext);
			deviceContext.pDeviceContext->Draw(static_cast<UINT>(chunk.vertexBufferSize), 0);
		}
	}
}

void WorldRenderer::Draw(Chunk& chunk, const SceneContext& sceneContext)
{
	const D3D11Context& deviceContext{ sceneContext.d3dContext };

	constexpr XMFLOAT4X4 worldMatrix{ 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);
	const auto viewProjection = XMLoadFloat4x4(&sceneContext.pCamera->GetViewProjection());

	m_pWorldVar->SetMatrix(reinterpret_cast<float*>(&world));
	m_pWvpVar->SetMatrix(reinterpret_cast<const float*>(&viewProjection));

	deviceContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	constexpr UINT offset = 0;
	constexpr UINT stride = sizeof(VertexPosNormTex);

	deviceContext.pDeviceContext->IASetVertexBuffers(0, 1, &chunk.pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, deviceContext.pDeviceContext);
		deviceContext.pDeviceContext->Draw(static_cast<UINT>(chunk.vertices.size()), 0);
	}
}
