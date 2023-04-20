#pragma once

#include "Utils/VertexHelper.h"
#include "WorldData.h"

#include <DirectXMath.h>
#include <vector>
#include <unordered_map>

struct XMINT3HashWrapper
{
	XMINT3HashWrapper(int x, int y, int z) : vector{ x,y,z } {}
	XMINT3HashWrapper(const XMINT3& _vector) : vector{ _vector } {}

	XMINT3 vector{};

	bool operator==(const XMINT3HashWrapper& other) const {
		return vector.x == other.vector.x && vector.y == other.vector.y && vector.z == other.vector.z;
	}
};

namespace std {
	template <>
	struct hash<XMINT3HashWrapper> {
		size_t operator()(const XMINT3HashWrapper& vw) const {
			return ((hash<int>()(vw.vector.x) ^ (hash<int>()(vw.vector.y) << 1)) >> 1) ^ (hash<int>()(vw.vector.z) << 1);
		}
	};
}

struct Water
{
	~Water()
	{
		SafeRelease(pVertexBuffer);
	}

	std::vector<VertexPosNormTex> vertices{};
	std::vector<WaterBlock*> pBlocks{};
	ID3D11Buffer* pVertexBuffer{};
	int vertexBufferSize{};
	bool verticesChanged{ true };
};