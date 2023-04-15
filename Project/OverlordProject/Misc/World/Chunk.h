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
	}

	std::vector<VertexPosNormTex> vertices{};
	std::vector<Block*> pBlocks{};
	ID3D11Buffer* pVertexBuffer{};
	XMINT2 position;
	int colliderIdx{ -1 };
	bool verticesChanged{ true };
	bool loadedWater{};
};