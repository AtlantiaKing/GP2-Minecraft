#pragma once

namespace MathHelper
{
	inline XMFLOAT3 QuaternionToEuler(XMFLOAT4 q)
	{
		XMFLOAT3 euler{};

		euler.y = atan2f(2.f * q.x * q.w + 2.f * q.y * q.z, 1.f - 2.f * (q.z * q.z + q.w * q.w));     // Yaw 
		euler.x = asinf(2.f * (q.x * q.z - q.w * q.y));                             // Pitch 
		euler.z = atan2f(2.f * q.x * q.y + 2.f * q.z * q.w, 1.f - 2.f * (q.y * q.y + q.z * q.z));      // Roll 

		return euler;
	}

	inline bool XMFloat4Equals(const XMFLOAT4& a, const XMFLOAT4& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
	}

	inline bool XMFloat3Equals(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}

	inline bool XMFloat2Equals(const XMFLOAT2& a, const XMFLOAT2& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	inline bool FloatEquals(float a, float b)
	{
		return abs(a - b) < FLT_EPSILON;
	}

	inline float randF(float min, float max)
	{
		const float random = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		const float diff = max - min;
		const float r = random * diff;
		return min + r;
	}

	inline LONG BinaryClamp(LONG x)
	{
		if (x > 0) return 1;
		if (x < 0) return -1;
		return 0;
	}

	template<typename T>
	void Clamp(T& value, T hi, T lo)
	{
		if (value > hi)
			value = hi;

		if (value < lo)
			value = lo;
	}

	inline XMFLOAT4 GetLookAtQuaternion(const XMFLOAT3& lookDirection)
	{
		const XMVECTOR forward{ XMVector3Normalize(XMLoadFloat3(&lookDirection)) };
		const XMVECTOR defaultForward{ 0.0f, 0.0f, 1.0f, 0.0f };

		XMVECTOR rotationAxis = XMVector3Normalize(XMVector3Cross(defaultForward, forward));

		const float dotProduct = XMVectorGetX(XMVector3Dot(defaultForward, forward));

		if (MathHelper::FloatEquals(dotProduct, -1.0f)) rotationAxis = { 0.0f, 1.0f, 0.0f };

		const float rotationAngle = acosf(dotProduct);
		XMVECTOR quaternionAxis = XMVectorScale(rotationAxis, sinf(rotationAngle / 2.0f));

		XMFLOAT4 quaternion;
		XMStoreFloat4(&quaternion, quaternionAxis);

		quaternion.w = cosf(rotationAngle / 2.0f);

		return quaternion;
	}
}