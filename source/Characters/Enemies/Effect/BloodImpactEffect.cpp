#include "BloodImpactEffect.h"

BloodImpactEffect::BloodImpactEffect(P2World& world, const Vec2& postion, ObjectBase& ownerClass, bool flipFlg)
	: ObjectBase(world, postion),
	  owner(&ownerClass)
{
	BloodImpactEffect::owner = &ownerClass;

	assetName = U"BloodImpact";
	margin = Vec2{ 0,0 };
	reSize = Vec2{ IMG_RESIZE,IMG_RESIZE };
	pos = postion;
	img_flipFlg = flipFlg;
}

BloodImpactEffect::~BloodImpactEffect()
{
}

void BloodImpactEffect::update()
{
	//現在の生存時間を更新
	lifeTime += Scene::DeltaTime();

	//画像切り替え
	imgChange_ct += Scene::DeltaTime();
	if (imgChange_ct >= IMG_CHANGE_TIME) {
		if (margin.x <= IMG_SIZE_X * 5) {
			margin.x += IMG_SIZE_X;
		}
		else {
			margin.x = 0;
			margin.y += IMG_SIZE_Y;
		}

		//最後の画像が描画されたか
		if (margin.x == 0 && margin.y == IMG_SIZE_Y * 3) {
			animationEndFlg = true;
		}
		imgChange_ct = 0.0;
	}

	if (animationEndFlg) {
		deleteSelf();
	}
}

void BloodImpactEffect::draw() const
{
	//画像の描画
	TextureAsset(assetName)(margin, IMG_SIZE_X, IMG_SIZE_Y).mirrored(img_flipFlg).resized(reSize).drawAt(pos);
}
