#include "stdafx.h"
#include "TileAtlas.h"

XMFLOAT2 TileAtlas::GetUV(BlockType blockType, const XMFLOAT2& originalUV) const
{
	// There are 16x16 tiles in the tile atlas
	constexpr int nrBlocksPerRow{ 16 };
	constexpr float tileSize{ 1.0f / 16.0f };
	constexpr float tileEpsilon{ 0.0001f };

	const int blockX{ static_cast<int>(blockType) % nrBlocksPerRow };
	const int blockY{ static_cast<int>(blockType) / nrBlocksPerRow };

	constexpr float halfUV{ 0.5f };
	const float epsilonX{ originalUV.x > halfUV ? -tileEpsilon : tileEpsilon };
	const float epsilonY{ originalUV.y > halfUV ? -tileEpsilon : tileEpsilon };

	XMFLOAT2 blockUV
	{
		originalUV.x * tileSize + epsilonX + tileSize * blockX,
		originalUV.y * tileSize + epsilonY + tileSize * blockY
	};

	return blockUV;
}
