#pragma once

#include "Utils/VertexHelper.h"
#include "WorldData.h"

#include <DirectXMath.h>
#include <vector>

struct Chunk
{
	~Chunk() 
	{
		SafeRelease(pVertexBuffer);
		SafeRelease(pVertexTransparentBuffer);
	}

	std::vector<VertexPosNormTexTransparency> vertices{};
	std::vector<BlockType> pBlocks{};

	XMINT2 position;

	ID3D11Buffer* pVertexBuffer{};
	ID3D11Buffer* pBackVertexBuffer{};
	ID3D11Buffer* pVertexTransparentBuffer{};
	ID3D11Buffer* pBackVertexTransparentBuffer{};
	
	int vertexBufferSize{};
	int backVertexBufferSize{};
	int vertexTransparentBufferSize{};
	int backVertexTransparentBufferSize{};

	int colliderIdx{ -1 };
	bool verticesChanged{ true };
	bool needColliderChange{ true };
	bool loadedWater{};
};