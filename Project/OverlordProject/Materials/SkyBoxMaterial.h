#pragma once
class SkyBoxMaterial final : public Material<SkyBoxMaterial>
{
public:
	SkyBoxMaterial();
	~SkyBoxMaterial() override = default;

	SkyBoxMaterial(const SkyBoxMaterial& other) = delete;
	SkyBoxMaterial(SkyBoxMaterial&& other) noexcept = delete;
	SkyBoxMaterial& operator=(const SkyBoxMaterial& other) = delete;
	SkyBoxMaterial& operator=(SkyBoxMaterial&& other) noexcept = delete;

	void SetTexture(const std::wstring& assetFile);

protected:
	void InitializeEffectVariables() override {};

private:
	TextureData* m_pTexture{};
};


