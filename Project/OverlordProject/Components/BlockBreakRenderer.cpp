#include "stdafx.h"
#include "BlockBreakRenderer.h"

#include "Content/ContentLoader.h"

BlockBreakRenderer::BlockBreakRenderer(const SceneContext& sceneContext)
{
	const std::vector<VertexPosTex> vertices
	{
		{ { 0.501f, 0.501f, 0.501f }, { 0.0f, 0.0f } },
		{ { -0.501f, 0.501f, 0.501f }, { 1.0, 0.0f } },
		{ { -0.501f, -0.501f, 0.501f }, { 1.0, 1.0 } },
		{ { 0.501f, 0.501f, 0.501f }, { 0.0f, 0.0f } },
		{ { -0.501f, -0.501f, 0.501f }, { 1.0, 1.0 } },
		{ { 0.501f, -0.501f, 0.501f }, { 0.0f, 1.0 } },

		{ { -0.501f, 0.501f, -0.501f }, { 0.0f, 0.0f } },
		{ { 0.501f, 0.501f, -0.501f }, { 1.0, 0.0f } },
		{ { 0.501f, -0.501f, -0.501f }, { 1.0, 1.0 } },
		{ { -0.501f, 0.501f, -0.501f }, { 0.0f, 0.0f } },
		{ { 0.501f, -0.501f, -0.501f }, { 1.0, 1.0 } },
		{ { -0.501f, -0.501f, -0.501f }, { 0.0f, 1.0 } },

		{ { 0.501f, 0.501f, -0.501f }, { 0.0f, 0.0f } },
		{ { 0.501f, 0.501f, 0.501f }, { 1.0, 0.0f } },
		{ { 0.501f, -0.501f, 0.501f }, { 1.0, 1.0 } },
		{ { 0.501f, 0.501f, -0.501f }, { 0.0f, 0.0f } },
		{ { 0.501f, -0.501f, 0.501f }, { 1.0, 1.0 } },
		{ { 0.501f, -0.501f, -0.501f }, { 0.0f, 1.0 } },

		{ { -0.501f, 0.501f, 0.501f }, { 0.0f, 0.0f } },
		{ { -0.501f, 0.501f, -0.501f }, { 1.0, 0.0f } },
		{ { -0.501f, -0.501f, -0.501f }, { 1.0, 1.0 } },
		{ { -0.501f, 0.501f, 0.501f }, { 0.0f, 0.0f } },
		{ { -0.501f, -0.501f, -0.501f }, { 1.0, 1.0 } },
		{ { -0.501f, -0.501f, 0.501f }, { 0.0f, 1.0 } },

		{ { -0.501f, 0.501f, 0.501f }, { 0.0f, 0.0f } },
		{ { 0.501f, 0.501f, 0.501f }, { 1.0, 0.0f } },
		{ { 0.501f, 0.501f, -0.501f }, { 1.0, 1.0 } },
		{ { -0.501f, 0.501f, 0.501f }, { 0.0f, 0.0f } },
		{ { 0.501f, 0.501f, -0.501f }, { 1.0, 1.0 } },
		{ { -0.501f, 0.501f, -0.501f }, { 0.0f, 1.0 } },

		{ { -0.501f, -0.501f, -0.501f }, { 0.0f, 0.0f } },
		{ { 0.501f, -0.501f, -0.501f }, { 1.0, 0.0f } },
		{ { 0.501f, -0.501f, 0.501f }, { 1.0, 1.0 } },
		{ { -0.501f, -0.501f, -0.501f }, { 0.0f, 0.0f } },
		{ { 0.501f, -0.501f, 0.501f }, { 1.0, 1.0 } },
		{ { -0.501f, -0.501f, 0.501f }, { 0.0f, 1.0 } }
	};


	m_NrVertices = static_cast<int>(vertices.size());

	auto& d3d11 = sceneContext.d3dContext;
	auto size = vertices.size();

	//*************
	//VERTEX BUFFER
	D3D11_BUFFER_DESC vertexBuffDesc{};
	vertexBuffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.ByteWidth = static_cast<UINT>(sizeof(VertexPosTex) * vertices.size());
	vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	vertexBuffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.MiscFlags = 0;

	sceneContext.d3dContext.pDevice->CreateBuffer(&vertexBuffDesc, nullptr, &m_pVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	d3d11.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mappedResource);
	memcpy(mappedResource.pData, vertices.data(), sizeof(VertexPosTex) * size);
	d3d11.pDeviceContext->Unmap(m_pVertexBuffer, 0);

	m_pEffect = ContentManager::Load<ID3DX11Effect>(L"Effects\\BreakingBlock.fx");
	m_pTechnique = m_pEffect->GetTechniqueByIndex(0);
	EffectHelper::BuildInputLayout(sceneContext.d3dContext.pDevice, m_pTechnique, &m_pInputLayout);

	if (!m_pWvpVar)
		m_pWvpVar = m_pEffect->GetVariableBySemantic("WorldViewProjection")->AsMatrix();

	if(!m_pStageVar)
		m_pStageVar = m_pEffect->GetVariableBySemantic("DestroyStage")->AsScalar();

	m_pEffect->GetVariableBySemantic("DiffuseTexture")->AsShaderResource()->SetResource(ContentManager::Load<TextureData>(L"Textures\\BreakingSprite.dds")->GetShaderResourceView());
}

BlockBreakRenderer::~BlockBreakRenderer()
{
	SafeRelease(m_pInputLayout);
	SafeRelease(m_pVertexBuffer);
}

void BlockBreakRenderer::SetBreakingStage(int stage) const
{
	if (m_pStageVar)
	{
		m_pStageVar->SetInt(stage);
	}
}

void BlockBreakRenderer::Initialize(const SceneContext& /*sceneContext*/)
{
}

void BlockBreakRenderer::Draw(const SceneContext& sceneContext)
{
	if (!m_CanRender) return;

	const D3D11Context& deviceContext{ sceneContext.d3dContext };

	const XMVECTOR worldMatrix{ XMLoadFloat3(&GetTransform()->GetWorldPosition()) };
	const XMMATRIX world = XMMatrixTranslationFromVector(worldMatrix);
	const auto viewProjection = XMLoadFloat4x4(&sceneContext.pCamera->GetViewProjection());

	const auto wvp = world * viewProjection;
	m_pWvpVar->SetMatrix(reinterpret_cast<const float*>(&wvp));

	deviceContext.pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	deviceContext.pDeviceContext->IASetInputLayout(m_pInputLayout);

	constexpr UINT offset = 0;
	constexpr UINT stride = sizeof(VertexPosTex);
	deviceContext.pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc{};
	m_pTechnique->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		m_pTechnique->GetPassByIndex(p)->Apply(0, deviceContext.pDeviceContext);
		deviceContext.pDeviceContext->Draw(static_cast<UINT>(m_NrVertices), 0);
	}
}
