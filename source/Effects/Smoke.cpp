#include "Smoke.h"

Smoke::Smoke(P2World& world, const Vec2& pos)
	: ObjectBase(world, pos)
{
	position = pos;
	reSize = IMG_RESIZE;
	margin = Vec2{ 0,0 };
}

Smoke::~Smoke()
{
}

void Smoke::update()
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

void Smoke::draw() const
{
	// 画像を滑らかに補完
	const ScopedRenderStates2D rs{ SamplerState::ClampNearest };
	//画像の描画
	TextureAsset(U"Smoke")(margin, IMG_SIZE, IMG_SIZE).resized(reSize).drawAt(position);
}
