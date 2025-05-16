# pragma once

# include <Siv3D.hpp>

//分割画像登録 path(画像のファイルパス) size(分割したい画像のサイズ)
Array<TextureRegion> LoadDivGraph(const FilePath& path, const Size& image_size)
{
	// パス存在確認
	if (!FileSystem::Exists(path)) { throw Error{ U"ファイルが存在しません: {}"_fmt(path) }; }
	// Textureの読み込み
	const Texture texture(path);
	if (texture.isEmpty()) { throw Error{ U"画像の読み込みに失敗しました: {}"_fmt(path) }; }
	// サイズのバリデーション
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
