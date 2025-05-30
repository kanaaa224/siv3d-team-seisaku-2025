# include "StageBackground.hpp"

StageBackground::StageBackground(P2World& world, const Vec2& position) : ObjectBase(world, position)
{
	left_position = Vec2(position);
	right_position = Vec2((position.x * -1), position.y);
}

void StageBackground::update()
{
	
}

void StageBackground::draw() const
{
	//左側
	TextureAsset(U"Stage 1 Background").resized(Scene::Size()).draw(left_position);
	//右側
	TextureAsset(U"Stage 1 Background").resized(Scene::Size()).draw(right_position);
}

void StageBackground::setCameraPos(Vec2 pos)
{
	camera_pos = pos;
}
