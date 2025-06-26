#include "Star.h"

Star::Star(P2World& world, const Vec2& pos)
	: ObjectBase(world, pos)
{
	position = pos;
	reSize = Random(IMG_SIZE, IMG_SIZE + 20);
	margin = Vec2{ 0,0 };
}

Star::~Star()
{
}

void Star::update()
{
	time_ct += Scene::DeltaTime();

	if (time_ct >= CHANGE_IMG) {
		margin.x += IMG_SIZE;
		if (margin.x >= IMG_SIZE * IMG_NUM) {
			deleteSelf();
		}

		time_ct = 0;
	}
}

void Star::draw() const
{
	//画像の描画
	TextureAsset(U"Star")(margin, IMG_SIZE, IMG_SIZE).resized(reSize).drawAt(position);
}
