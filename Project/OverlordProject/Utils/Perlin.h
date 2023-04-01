#pragma once

class Perlin final
{
public:
	Perlin(int nrOctaves = 5, float zoom = 10.0f, const XMFLOAT2& offset = { 0.0f, 0.0f });
	~Perlin() = default;

	void SetOffset(const XMFLOAT2& offset) { m_Offset = offset; }
	void SetOctaves(int nrOctaves);
	void SetZoom(float zoom) { m_Zoom = zoom; }

	/*
	 Returns the noise value at x,y
	Noise value is always between 0 and 1
	*/
	float GetNoise(float x, float y) const;
private:
	float GetOctaveNoise(float x, float y, int octave) const;
	XMVECTOR GetRandomGradient(int ix, int iy) const;
	float Lerp(float a, float b, float t) const;

	float m_MaxValue{};
	XMFLOAT2 m_Offset{};
	int m_NrOctaves{};
	float m_Zoom{};
};

