#include "stdafx.h"
#include "BlockRenderer.h"

#include "Misc/World/TileAtlas.h"

BlockRenderer::BlockRenderer(BlockType type, const SceneContext& sceneContext)
{
	m_enableShadowMapDraw = true;

	TileAtlas tileAltas{};

	std::vector<VertexPosNormTexTransparency> vertices =
	{
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::FORWARD), { 0.0f, 0.0f }) },
		{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::FORWARD), { 1.0, 0.0f }) },
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::FORWARD), { 1.0, 1.0 }) },
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::FORWARD), { 0.0f, 0.0f }) },
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::FORWARD), { 1.0, 1.0 }) },
		{ { 0.5f, -0.5f, 0.5f }, { 0.0f, 0.0f, 1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::FORWARD), { 0.0f, 1.0 }) },

		{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BACK), { 0.0f, 0.0f }) },
		{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BACK), { 1.0, 0.0f }) },
		{ { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BACK), { 1.0, 1.0 }) },
		{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BACK), { 0.0f, 0.0f }) },
		{ { 0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BACK), { 1.0, 1.0 }) },
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BACK), { 0.0f, 1.0 }) },

		{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::RIGHT), { 0.0f, 0.0f }) },
		{ { 0.5f, 0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f },tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::RIGHT), { 1.0, 0.0f }) },
		{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::RIGHT), { 1.0, 1.0 }) },
		{ { 0.5f, 0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::RIGHT), { 0.0f, 0.0f }) },
		{ { 0.5f, -0.5f, 0.5f }, { 1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::RIGHT), { 1.0, 1.0 }) },
		{ { 0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::RIGHT), { 0.0f, 1.0 }) },

		{ { -0.5f, 0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::LEFT), { 0.0f, 0.0f }) },
		{ { -0.5f, 0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f },tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::LEFT), { 1.0, 0.0f }) },
		{ { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::LEFT), {  1.0, 1.0 }) },
		{ { -0.5f, 0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::LEFT), {  0.0f, 0.0f }) },
		{ { -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::LEFT), {  1.0, 1.0 }) },
		{ { -0.5f, -0.5f, 0.5f }, { -1.0f, 0.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::LEFT), {  0.0f, 1.0 }) },

		{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::UP), {  0.0f, 0.0f }) },
		{ { 0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::UP), {  1.0, 0.0f }) },
		{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::UP), {  1.0, 1.0 }) },
		{ { -0.5f, 0.5f, 0.5f }, { 0.0f, 1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::UP), {  0.0f, 0.0f }) },
		{ { 0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::UP), {  1.0, 1.0 }) },
		{ { -0.5f, 0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::UP), { 0.0f, 1.0 }) },

		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BOTTOM), { 0.0f, 0.0f })  },
		{ { 0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BOTTOM), { 1.0, 0.0f }) },
		{ { 0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BOTTOM), { 1.0, 1.0 }) } ,
		{ { -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BOTTOM), { 0.0f, 0.0f }) },
		{ { 0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BOTTOM), { 1.0, 1.0 }) } ,
		{ { -0.5f, -0.5f, 0.5f }, { 0.0f, -1.0f, 0.0f }, tileAltas.GetUV(tileAltas.GetFaceType(type, FaceDirection::BOTTOM), { 0.0f, 1.0 }) }
	};

	if (vertices.size() == 0) return;

	m_NrVertices = static_cast<int>(vertices.size());

	auto& d3d11 = sceneContext.d3dContext;
	auto size = vertices.size();

	//*************
	//VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBuffDesc{};
	vertexBuffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.ByteWidth = static_cast<UINT>(sizeof(VertexPosNormTexTransparency) * vertices.size());
	vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	vertexBuffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.MiscFlags = 0;

	sceneContext.d3dContext.pDevice->CreateBuffer(&vertexBuffDesc, nullptr, &m_pVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	d3d11.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	memcpy(mappedResource.pData, vertices.data(), sizeof(VertexPosNormTexTransparency) * size);
	d3d11.pDeviceContext->Unmap(m_pVertexBuffer, 0);

	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Effects\\World.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	EffectHelper::BuildInputLayout(sceneContext.d3dContext.pDevice, m_pTechnique, &m_pInputLayout);

	if (!m_pWorldVar)
		m_pWorldVar = m_pEffect->GetVariableBySemantic("World")->AsMatrix();

	if (!m_pWvpVar)
		m_pWvpVar = m_pEffect->GetVariableBySemantic("WorldViewProjection")->AsMatrix();

	m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource()->SetResource(ContentManager::Load<TextureData>(L"Textures\\TileAtlas.dds")->GetShaderResourceView());
}

BlockRenderer::~BlockRenderer()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void BlockRenderer::Draw(const SceneContext& sceneContext)
{
	const D3D11Context& deviceContext{ sceneContext.d3dContext };

	const XMMATRIX world = XMLoadFloat4x4(&GetTransform()->GetWorld());
	const auto viewProjection = XMLoadFloat4x4(&sceneContext.pCamera->GetViewProjection());

	const auto wvp = world * viewProjection;
	m_pWorldVar->SetMatrix(reinterpret_cast<const float*>(&world));
	m_pWvpVar->SetMatrix(reinterpret_cast<const float*>(&wvp));

	deviceContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	constexpr UINT offset = 0;
	constexpr UINT stride = sizeof(VertexPosNormTexTransparency);
	deviceContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, deviceContext.pDeviceContext);
		deviceContext.pDeviceContext->Draw(static_cast<UINT>(m_NrVertices), 0);
	}
}

void BlockRenderer::ShadowMapDraw(const SceneContext& sceneContext)
{
	constexpr int nrVertices{ 6 * 6 };
	constexpr UINT stride = sizeof(VertexPosNormTexTransparency);
	ShadowMapRenderer::Get()->DrawMesh(sceneContext, m_pVertexBuffer, nrVertices, stride, GetTransform()->GetWorld());
}
