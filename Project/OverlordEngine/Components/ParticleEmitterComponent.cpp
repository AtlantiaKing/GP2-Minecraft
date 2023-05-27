#include "stdafx.h"
#include "ParticleEmitterComponent.h"
#include "Misc/ParticleMaterial.h"

ParticleMaterial* ParticleEmitterComponent::m_pParticleMaterial{};

ParticleEmitterComponent::ParticleEmitterComponent(const std::wstring& assetFile, const ParticleEmitterSettings& emitterSettings, UINT particleCount):
	m_ParticlesArray(new Particle[particleCount]),
	m_ParticleCount(particleCount), //How big is our particle buffer?
	m_MaxParticles(particleCount), //How many particles to draw (max == particleCount)
	m_AssetFile(assetFile),
	m_EmitterSettings(emitterSettings)
{
	m_enablePostDraw = true; //This enables the PostDraw function for the component
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
	delete[] m_ParticlesArray;

	SafeRelease(m_pVertexBuffer);
}

void ParticleEmitterComponent::Initialize(const SceneContext& sceneContext)
{
	// Use the MaterialManager to create an instance of the ParticleMaterial
	if(!m_pParticleMaterial) m_pParticleMaterial = MaterialManager::Get()->CreateMaterial<ParticleMaterial>();

	// Call CreateVertexBuffer()
	CreateVertexBuffer(sceneContext);

	// Use the ContentManager to load the particle texture and store it in m_pParticleTexture
	m_pParticleTexture = ContentManager::Load<TextureData>(m_AssetFile);
}

void ParticleEmitterComponent::CreateVertexBuffer(const SceneContext& sceneContext)
{
	// If m_pVertexBuffer exists, release it
	SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC vertexBuffDesc{};
	vertexBuffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	vertexBuffDesc.ByteWidth = sizeof(VertexParticle) * m_ParticleCount;
	vertexBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	vertexBuffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	vertexBuffDesc.MiscFlags = 0;

	HRESULT vBufferResult{ sceneContext.d3dContext.pDevice->CreateBuffer(&vertexBuffDesc, nullptr, &m_pVertexBuffer) };
	HANDLE_ERROR(vBufferResult);
}

void ParticleEmitterComponent::Update(const SceneContext& sceneContext)
{
	// Cache the elapsed sec
	const float elapsedSec{ sceneContext.pGameTime->GetElapsed() };

	// Create a local variable, called particleInterval of type float.
	// This variable needs to contain the average particle emit threshold.
	const float averageEnergy{ (m_EmitterSettings.maxEnergy + m_EmitterSettings.minEnergy) / 2.0f };
	const float particleInterval{ averageEnergy / m_ParticleCount };

	// Increase m_LastParticleInit by the elapsed GameTime
	m_LastParticleSpawn += elapsedSec;

	// Map our vertexbuffer
	D3D11_MAPPED_SUBRESOURCE subResource;
	sceneContext.d3dContext.pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);

	// Create an empty pointer of type VertexParticle (ParticleVertex* pBuffer) and cast & assign pData to it.
	VertexParticle* pBuffer{ reinterpret_cast<VertexParticle*>(subResource.pData) };

	// Iterate the Particle Array
	m_ActiveParticles = 0;
	for (UINT i{}; i < m_ParticleCount; ++i)
	{
		Particle& particle{ m_ParticlesArray[i] };

		// If the particle is currently Active > UpdateParticle function.
		if (particle.isActive) UpdateParticle(particle, elapsedSec);

		// If the particle is NOT Active (do not use ‘else if’), and the value of m_LastParticleInit is
		//	bigger than or equal to particleInterval > Call SpawnParticle.
		if (!particle.isActive && (m_LastParticleSpawn >= particleInterval || m_EmitterSettings.burst))
		{
			SpawnParticle(particle);
			m_LastParticleSpawn -= particleInterval;
		}

		// If (after potential update and/or spawn) the particle is Active, add the particle to the VertexBuffer
		if (particle.isActive)
		{
			pBuffer[m_ActiveParticles] = particle.vertexInfo;

			++m_ActiveParticles;
		}
	}

	// Use DeviceContext::Unmap to unmap our vertexbuffer.
	sceneContext.d3dContext.pDeviceContext->Unmap(m_pVertexBuffer, 0);
}

void ParticleEmitterComponent::UpdateParticle(Particle& p, float elapsedTime) const
{
	// Check if the particle is active, if not, return
	if (!p.isActive) return;

	// Subtract the elapsedTime from the particle’s currentEnergy. 
	p.currentEnergy -= elapsedTime;
	// If currentEnergy is smaller than ZERO, deactivate the particle and return
	if (p.currentEnergy < 0)
	{
		p.isActive = false;
		return;
	}

	p.velocity.y += m_EmitterSettings.gravity * elapsedTime;

	// Add the velocity (m_EmitterSettings) multiplied by the elapsedTime, this way our
	//		particle moves in the direction of the velocity defined by the emitter settings.
	XMStoreFloat3(&p.vertexInfo.Position, XMLoadFloat3(&p.vertexInfo.Position) + XMLoadFloat3(&p.velocity) * elapsedTime);

	// Create a local variable, called ‘lifePercent of type float, this is the percentual particle lifetime
	const float lifePercent{ p.currentEnergy / p.totalEnergy };

	// Our color equals the color given by the emitter settings
	p.vertexInfo.Color = m_EmitterSettings.color;

	// The alpha value of the particle color should fade out over time.
	constexpr float fadeDelay{ 2.0f };
	p.vertexInfo.Color.w = m_EmitterSettings.color.w * lifePercent * fadeDelay;

	// Based on the particle’s sizeChange value, our particle shrinks or grows over time.
	constexpr float unitSize{ 1.0f };
	p.vertexInfo.Size = p.initialSize * (unitSize + (p.sizeChange - unitSize) * (1.0f - lifePercent));
}

void ParticleEmitterComponent::SpawnParticle(Particle& p)
{
	// Set particle’s isActive to true
	p.isActive = true;

	// totalEnergy and currentEnergy of the particle are both equal to a random float between
	//		minEnergy and maxEnergy (see m_EmitterSettings)
	p.totalEnergy = MathHelper::randF(m_EmitterSettings.minEnergy, m_EmitterSettings.maxEnergy);
	p.currentEnergy = p.totalEnergy;

	// We need to calculate a random position; this position is determined by the emitter radius
	//		of our particle system.
	const XMVECTOR randomVec
	{ 
		XMVector3Normalize(
		{
			MathHelper::randF(-1.0f, 1.0f),
			MathHelper::randF(-1.0f, 1.0f),
			MathHelper::randF(-1.0f, 1.0f)
		}) 
	};
	const float randomRange{ MathHelper::randF(m_EmitterSettings.minEmitterRadius, m_EmitterSettings.maxEmitterRadius) };
	XMStoreFloat3(&p.vertexInfo.Position, XMLoadFloat3(&GetTransform()->GetWorldPosition()) + randomVec * randomRange);

	// Our vertexInfo.Size and initialSize are both equal to a random value that lays between
	//		MinSize and MaxSize (see EmitterSettings)
	p.initialSize = MathHelper::randF(m_EmitterSettings.minSize, m_EmitterSettings.maxSize);
	p.vertexInfo.Size = p.initialSize;

	// sizeChange is equal to a random value that lays between minScale and maxScale 
	//		(see EmitterSettings)
	p.sizeChange = MathHelper::randF(m_EmitterSettings.minScale, m_EmitterSettings.maxScale);

	// The rotation (vertexInfo.Rotation) is a random value between –PI and PI.
	p.vertexInfo.Rotation = MathHelper::randF(-XM_PI, XM_PI);

	// The particle’s color (vertexInfo.Color) is equal to the color from the emitter settings
	p.vertexInfo.Color = m_EmitterSettings.color;

	p.velocity =
	{
		MathHelper::randF(m_EmitterSettings.minVelocity.x, m_EmitterSettings.maxVelocity.x),
		MathHelper::randF(m_EmitterSettings.minVelocity.y, m_EmitterSettings.maxVelocity.y),
		MathHelper::randF(m_EmitterSettings.minVelocity.z, m_EmitterSettings.maxVelocity.z)
	};
}

void ParticleEmitterComponent::PostDraw(const SceneContext& sceneContext)
{
	// Set the following shader variables to our ParticleMaterial (m_pParticleMaterial) 
	m_pParticleMaterial->SetVariable_Matrix(L"gWorldViewProj", sceneContext.pCamera->GetViewProjection());
	m_pParticleMaterial->SetVariable_Matrix(L"gViewInverse", sceneContext.pCamera->GetViewInverse());
	m_pParticleMaterial->SetVariable_Texture(L"gParticleTexture", m_pParticleTexture->GetShaderResourceView());
	m_pParticleMaterial->SetVariable_Vector(L"gSpriteSize", m_EmitterSettings.spriteSize);
	m_pParticleMaterial->SetVariable_Vector(L"gSpritePivot", m_EmitterSettings.spritePivot);

	// Retrieve the TechniqueContext from the material, this structure contains relevant information to
	//		setup the pipeline(BaseMaterial::GetTechniqueContext)
	const MaterialTechniqueContext& techContext{ m_pParticleMaterial->GetTechniqueContext() };

	ID3D11DeviceContext* deviceContext{ sceneContext.d3dContext.pDeviceContext };

	// Set the InputLayout
	deviceContext->IASetInputLayout(techContext.pInputLayout);

	// Set the PrimitiveTopology
	deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Set the VertexBuffer
	constexpr UINT offset{}, startSlot{}, nrBuffers{ 1 };
	constexpr UINT stride{ sizeof(VertexParticle) };
	deviceContext->IASetVertexBuffers(startSlot, nrBuffers, &m_pVertexBuffer, &stride, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	techContext.pTechnique->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		// Apply the pass
		techContext.pTechnique->GetPassByIndex(p)->Apply(0, deviceContext);
		// Draw the vertices! (The number of vertices we want to draw is equal to m_ActiveParticles
		deviceContext->Draw(m_ActiveParticles, 0);
	}
}