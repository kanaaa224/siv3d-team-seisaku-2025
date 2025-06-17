#include "CloudOfDustEffect.h"

CloudOfDustEffect::CloudOfDustEffect(P2World& world, const Vec2& postion, bool flipFlg)
	: ObjectBase(world,postion)
{
	assetName = U"Effect 1 27";
	margin = Vec2{ 0.0,0.0 };
	reSize = Vec2{ IMG_RESIZE ,IMG_RESIZE };
	if (flipFlg == true) {
		img_flipFlg = false;
	}
	else {
		img_flipFlg = true;
	}

	pos = postion;
	imgChange_ct = 0.0;
}

CloudOfDustEffect::~CloudOfDustEffect()
{
}

void CloudOfDustEffect::update()
{
	//現在の生存時間を更新
	lifeTime += Scene::DeltaTime();

	//画像切り替え
	imgChange_ct += Scene::DeltaTime();
	if (imgChange_ct >= IMG_CHANGE_TIME) {
		if (margin.x != 192 - IMG_SIZE) {//192は画像を分割してない時のサイズ
			margin.x += IMG_SIZE;
		}
		else//画像の２段目に切り替える
		{
			margin.x = 0.0;
			margin.y += IMG_SIZE;
		}

		//最後の画像が描画されたか
		if (margin.x == 192 - IMG_SIZE && margin.y == 32) {
			animationEndFlg = true;
		}
		imgChange_ct = 0.0;
	}

	if (animationEndFlg) {
		deleteSelf();
	}
}

void CloudOfDustEffect::draw() const
{
	//画像の描画
	TextureAsset(assetName)(margin, IMG_SIZE, IMG_SIZE).mirrored(img_flipFlg).resized(reSize).drawAt(pos);
}
