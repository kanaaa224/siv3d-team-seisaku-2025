#include "ExclamationMarkEffect.h"
#include "../EnemyBase.hpp"

ExclamationMarkEffect::ExclamationMarkEffect(P2World& world, const Vec2& postion, ObjectBase& ownerClass)
	: ObjectBase(world,postion),
	  owner(&ownerClass)
{
	ExclamationMarkEffect::owner = &ownerClass;

	assetName = U"Excalimation Mark";
	margin = Vec2{ 0.0,0.0 };
	reSize = Vec2{ IMG_RESIZE ,IMG_RESIZE };
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
	//描画位置の更新
	if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(owner)) {
		pos = enemy->getPosition() + enemy->getDrawExcMarkPos();
	}

	//画像切り替え
	imgChange_ct += Scene::DeltaTime();
	if (imgChange_ct >= IMG_CHANGE_TIME) {
		margin.y += IMG_SIZE;

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
	TextureAsset(assetName)(margin, IMG_SIZE, IMG_SIZE).resized(reSize).drawAt(pos);
}
