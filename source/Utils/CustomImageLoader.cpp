# include "CustomImageLoader.hpp"

Array<TextureRegion> LoadDivGraph(const AssetNameView& name, const Size& image_size)
{
	TextureAsset texture = TextureAsset(name);

	if (image_size.x <= 0 || image_size.y <= 0) { throw Error{ U"切り出しサイズが不正です: {}"_fmt(image_size) }; }

	// 警告：端が余るかも
	if ((texture.width() % image_size.x != 0) || (texture.height() % image_size.y != 0)) { Print << U"警告: 切り出しサイズが画像とぴったり一致していません"; }

	Array<TextureRegion> textureRegions;

	for (auto p : step(Size(texture.width() / image_size.x, texture.height() / image_size.y)))
	{
		textureRegions.push_back(texture(p * image_size, image_size));
	}

	return textureRegions;
}
