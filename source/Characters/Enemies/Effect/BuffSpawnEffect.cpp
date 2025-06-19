#include "BuffSpawnEffect.h"

BuffSpawnEffect::BuffSpawnEffect(P2World& world, const Vec2& postion, ObjectBase& ownerClass, String color)
	: ObjectBase(world, postion),
	  owner(&ownerClass)
{
	BuffSpawnEffect::owner = &ownerClass;

	assetName = U"BuffSpawn";
	margin = setEffectColor(color);
	reSize = Vec2{ IMG_RESIZE,IMG_RESIZE };
	pos = postion;
	imgChange_ct = 0.0;
}

BuffSpawnEffect::~BuffSpawnEffect()
{
}

void BuffSpawnEffect::update()
{
	//現在の生存時間を更新
	lifeTime += Scene::DeltaTime();

	//画像切り替え
	imgChange_ct += Scene::DeltaTime();
	if (imgChange_ct >= IMG_CHANGE_TIME) {
		if (margin.x <= 1088) {
			margin.x += IMG_SIZE;
		}

		//最後の画像が描画されたか
		if (margin.x == 1152) {
			animationEndFlg = true;
		}
		imgChange_ct = 0.0;
	}

	if (animationEndFlg) {
		deleteSelf();
	}
}

void BuffSpawnEffect::draw() const
{
	//画像の描画
	TextureAsset(assetName)(margin, IMG_SIZE, IMG_SIZE).mirrored(img_flipFlg).resized(reSize).drawAt(pos);
}

Vec2 BuffSpawnEffect::setEffectColor(String color)
{
	if (color == U"white") {
		return Vec2{ 0,BUFF_EFFECT_COLOR_WHITE };
	}
	if (color == U"red") {
		return Vec2{ 0,BUFF_EFFECT_COLOR_RED };
	}
	if (color == U"puprle") {
		return Vec2{ 0,BUFF_EFFECT_COLOR_PURPLE };
	}
	if (color == U"blue") {
		return Vec2{ 0,BUFF_EFFECT_COLOR_BLUE };
	}
	if (color == U"green") {
		return Vec2{ 0,BUFF_EFFECT_COLOR_GREEN };
	}
	if (color == U"orange") {
		return Vec2{ 0,BUFF_EFFECT_COLOR_ORANGE };
	}

	//デフォルトで白色
	return Vec2{ 0,BUFF_EFFECT_COLOR_WHITE };
}
