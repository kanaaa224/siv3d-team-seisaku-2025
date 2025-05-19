# pragma once

# include <Siv3D.hpp>

// 分割画像登録: name(画像のアセット名), size(分割したい画像のサイズ)
Array<TextureRegion> LoadDivGraph(const AssetNameView& name, const Size& image_size);
