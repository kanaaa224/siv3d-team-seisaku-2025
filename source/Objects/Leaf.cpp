#include "Leaf.h"

Leaf::Leaf(P2World& world, const Vec2& position, int imgNum, eDropPostion dropPos, double windStrength)
	: ObjectBase(world, position)
{
	body = world.createCircle(P2Dynamic, position,0.1);

	body.setGravityScale(10);

	//画像の左上座標の設定
	if (imgNum >= 0 || imgNum <= 4) {
		margin = Vec2{ LEAF_IMG_SIZE * imgNum,0.0 };
	}
	else {
		margin = Vec2{ 0.0,0.0 };
	}

	reSize = Random(RESIZE_MIN, RESIZE_MAX);

	//風の強さ(まっすぐ落ちる場合は影響しない)
	windStr = windStrength;

	//風向きの設定
	switch (dropPos)
	{
	case eLeft://左下に落ちる
		windDirection = Vec2{ -windStr,body.getGravityScale() };
		break;
	case eCenter://まっすぐ落ちる
		windDirection = Vec2{ 0.0,body.getGravityScale() };
		break;
	case eRight://右下に落ちる
		windDirection = Vec2{ windStr,body.getGravityScale() };
		break;
	default:
		break;
	}

	body.setVelocity(windDirection);

	lifeTime = 0.0;
	timeOffset = Random(0, 5);
	spawnPos = position;
}

Leaf::~Leaf()
{
}

void Leaf::update()
{
	//生存時間の更新
	lifeTime += Scene::DeltaTime();

	movement();

	//画面下に来たら自分を削除
	if (body.getPos().y >= Scene::Size().y + LEAF_IMG_SIZE) {
		deleteSelf();
	}
}

void Leaf::draw() const
{
	TextureAsset(U"Leaf")(margin, LEAF_IMG_SIZE, LEAF_IMG_SIZE).resized(reSize).drawAt(body.getPos());
}

void Leaf::movement()
{
	windDirection.x = Math::Sin(lifeTime + timeOffset) * windStr;

	body.setVelocity(windDirection);
}
