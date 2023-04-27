#pragma once
#include "WorldData.h"

class TileAtlas final
{
public:
	XMFLOAT2 GetUV(FaceType blockType, const XMFLOAT2& originalUV) const;
	FaceType GetFaceType(BlockType blockType, FaceDirection faceDirection) const;
private:
};

