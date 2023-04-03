#include "stdafx.h"
#include "WorldComponent.h"

WorldComponent::WorldComponent(const SceneContext& sceneContext)
{
	m_Renderer.LoadEffect(sceneContext);
}

void WorldComponent::Initialize(const SceneContext& sceneContext)
{
    m_pRb = GetGameObject()->AddComponent(new RigidBodyComponent{true});

	m_Renderer.SetBuffer(m_Generator.LoadWorld(), sceneContext);

	std::vector<XMFLOAT3> vertices{ m_Generator.GetVertices() };

	std::vector<PxU32> indices{};
	indices.reserve(vertices.size());
	for (PxU32 i{}; i < static_cast<PxU32>(vertices.size()); ++i)
	{
		indices.emplace_back(i);
	}

	auto& physX{ PxGetPhysics() };
    auto pPhysMat{ physX.createMaterial(0.0f, 0.0f, 1.0f) };

    // Create the cooking interface
    PxCooking* cooking = PxCreateCooking(PX_PHYSICS_VERSION, physX.getFoundation(), PxCookingParams{ PxTolerancesScale{} });

    // Create the triangle mesh desc
    PxTriangleMeshDesc meshDesc;
    meshDesc.points.count = static_cast<PxU32>(vertices.size());
    meshDesc.points.stride = sizeof(PxVec3);
    meshDesc.points.data = vertices.data();
    meshDesc.triangles.count = static_cast<PxU32>(vertices.size()) / 3;
    meshDesc.triangles.stride = 3 * sizeof(PxU32);
    meshDesc.triangles.data = indices.data();

    // Cook the mesh
    PxDefaultMemoryOutputStream writeBuffer;
    cooking->cookTriangleMesh(meshDesc, writeBuffer);

    // Create the triangle mesh
    PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
    PxTriangleMesh* triangleMesh = physX.createTriangleMesh(readBuffer);

    std::cout << triangleMesh << "\n";

    PxTriangleMeshGeometry geometry{ triangleMesh };

    // Cleanup
    cooking->release();

    m_pRb->AddCollider(geometry, *pPhysMat);
}

void WorldComponent::Draw(const SceneContext& sceneContext)
{
	m_Renderer.Draw(sceneContext);
}
