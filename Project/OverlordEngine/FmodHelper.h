#pragma once

namespace FmodHelper
{
	inline FMOD_VECTOR ToFmod(const XMFLOAT3& v)//DirectX vector to FMOD vector
	{
		return FMOD_VECTOR(v.x, v.y, v.z);
	}

	inline FMOD_VECTOR ToFmod(const PxVec3& v)//PhysX vector to FMOD vector
	{
		return FMOD_VECTOR(v.x, v.y, v.z);
	}
}