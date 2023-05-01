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
	std::vector<Block*> pBlocks{};
	ID3D11Buffer* pVertexBuffer{};
	int vertexBufferSize{};
	ID3D11Buffer* pVertexTransparentBuffer{};
	int vertexTransparentBufferSize{};
	XMINT2 position;
	int colliderIdx{ -1 };
	bool verticesChanged{ true };
	bool loadedWater{};
};