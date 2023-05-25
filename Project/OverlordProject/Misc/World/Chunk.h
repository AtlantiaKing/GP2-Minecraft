#pragma once

#include "Utils/VertexHelper.h"
#include "WorldData.h"

#include <DirectXMath.h>
#include <vector>

struct Chunk
{
	void DeleteChunk() 
	{
		SafeRelease(pVertexBuffer);
		SafeRelease(pVertexTransparentBuffer);
	}

	std::vector<VertexPosNormTexTransparency> vertices{};
	std::vector<BlockType> pBlocks{};

	XMINT2 position;

	ID3D11Buffer* pVertexBuffer{};
	ID3D11Buffer* pVertexTransparentBuffer{};
	
	int vertexBufferSize{};
	int vertexTransparentBufferSize{};

	int colliderIdx{ -1 };
	bool verticesChanged{ true };
	bool needColliderChange{ true };
	bool loadedWater{};
};