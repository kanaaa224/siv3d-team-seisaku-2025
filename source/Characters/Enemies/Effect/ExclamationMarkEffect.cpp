#include "ExclamationMarkEffect.h"

ExclamationMarkEffect::ExclamationMarkEffect(P2World& world, const Vec2& postion, bool flipFlg)
	: ObjectBase(world,postion)
{
	assetName = U"Excalimation Mark";
	margin = Vec2{ 0.0,0.0 };
	reSize = Vec2{ IMG_RESIZE ,IMG_RESIZE };
	img_flipFlg = flipFlg;
	/*if (flipFlg == true) {
		img_flipFlg = false;
	}
	else {
		img_flipFlg = true;
	}*/

	pos = postion;
	imgChange_ct = 0.0;
}

ExclamationMarkEffect::~ExclamationMarkEffect()
{
}

void ExclamationMarkEffect::update()
{
	//現在の生存時間を更新
	lifeTime += Scene::DeltaTime();

	//画像切り替え
	imgChange_ct += Scene::DeltaTime();
	if (imgChange_ct >= IMG_CHANGE_TIME) {
		if (margin.y != 128 - IMG_SIZE) {//128は画像を分割してない時のサイズ
			margin.y += IMG_SIZE;
		}

		//最後の画像が描画されたか
		if (margin.y == 128) {
			animationEndFlg = true;
		}
		imgChange_ct = 0.0;
	}

	if (animationEndFlg) {
		deleteSelf();
	}
}

void ExclamationMarkEffect::draw() const
{
	//画像の描画
	TextureAsset(assetName)(margin, IMG_SIZE, IMG_SIZE).mirrored(img_flipFlg).resized(reSize).drawAt(pos);
}
