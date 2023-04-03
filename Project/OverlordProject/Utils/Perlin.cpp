#include "stdafx.h"
#include "Perlin.h"

Perlin::Perlin(int nrOctaves, float zoom, const XMFLOAT2& offset)
	: m_Zoom{ zoom }
	, m_Offset{ offset }
{
	SetOctaves(nrOctaves);
}

void Perlin::SetOctaves(int nrOctaves)
{
	m_OctaveSeeds.clear();

	m_NrOctaves = nrOctaves;
	m_MaxNoiseValue = 0.0f;
	for (int i{ 1 }; i <= nrOctaves; ++i)
	{
		m_MaxNoiseValue += 1.0f / i;
		m_OctaveSeeds.push_back(XMFLOAT2{ (rand() % 1001 / 1000.0f) * 10000, (rand() % 1001 / 1000.0f) * 10000 });
	}
}

float Perlin::GetNoise(float x, float y) const
{
	float noise{};
	for (int i{ 1 }; i <= m_NrOctaves; ++i)
	{
		noise += GetOctaveNoise(x, y, i) / i;
	}
	noise /= (m_MaxNoiseValue / 2.0f);
	noise /= 2.0f;
	noise += 0.5f;

	return noise;
}

float Perlin::GetOctaveNoise(float x, float y, int octave) const
{
	x += m_MiddleOfNoise + m_OctaveSeeds[octave-1].x + m_Offset.x;
	y += m_MiddleOfNoise + m_OctaveSeeds[octave-1].y + m_Offset.y;

	x *= powf(2.0f, static_cast<float>(octave - 1)) / m_Zoom;
	y *= powf(2.0f, static_cast<float>(octave - 1)) / m_Zoom;

	const int gridX0{ static_cast<int>(x) };
	const int gridX1{ gridX0 + 1 };
	const int gridY0{ static_cast<int>(y) };
	const int gridY1{ gridY0 + 1 };

	const float gridPosX{ x - gridX0 };
	const float gridPosY{ y - gridY0 };

	const float dotGradient0{ XMVector2Dot(GetRandomGradient(gridX0, gridY0), XMVECTOR{ gridPosX, gridPosY }).m128_f32[0] };
	const float dotGradient1{ XMVector2Dot(GetRandomGradient(gridX1, gridY0), XMVECTOR{ gridPosX - 1.0f, gridPosY }).m128_f32[0] };
	const float dotGradient2{ XMVector2Dot(GetRandomGradient(gridX0, gridY1), XMVECTOR{ gridPosX, gridPosY - 1.0f }).m128_f32[0] };
	const float dotGradient3{ XMVector2Dot(GetRandomGradient(gridX1, gridY1), XMVECTOR{ gridPosX - 1.0f, gridPosY - 1.0f }).m128_f32[0] };

	const float xEase{ 3.0f * powf(gridPosX, 2.0f) - 2.0f * powf(gridPosX, 3.0f) };
	const float yEase{ 3.0f * powf(gridPosY, 2.0f) - 2.0f * powf(gridPosY, 3.0f) };

	return Lerp(Lerp(dotGradient0, dotGradient1, xEase), Lerp(dotGradient2, dotGradient3, xEase), yEase);
}

XMVECTOR Perlin::GetRandomGradient(int ix, int iy) const
{
	// No precomputed gradients mean this works for any number of grid coordinates
	const unsigned w = 8 * sizeof(unsigned);
	const unsigned s = w / 2; // rotation width
	unsigned a = ix, b = iy;
	a *= 3284157443; 
	b ^= (a << s) | (a >> (w - s));
	b *= 1911520717; 
	a ^= (b << s) | (b >> (w - s));
	a *= 2048419325;
	const float random = static_cast<float>(a * (XM_PI / ~(~0u >> 1))); // in [0, 2*Pi]
	const XMVECTOR v
	{
		cosf(random),
		sinf(random)
	};
	return v;
}

float Perlin::Lerp(float a, float b, float t) const
{
	return a + t * (b - a);
}
