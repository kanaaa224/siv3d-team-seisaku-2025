# include "StageBackground.hpp"

StageBackground::StageBackground(P2World& world, const Vec2& position) : ObjectBase(world, position) {}

void StageBackground::update()
{
	const int width = Scene::Width();

	int offsetX = static_cast<int>(std::floor(camera_pos.x / width)) * width;

	texture_1_pos.x = offsetX - width;
	texture_2_pos.x = offsetX;
	texture_3_pos.x = offsetX + width;
}

void StageBackground::draw() const
{
	const auto texture = TextureAsset(U"Stage 1 Background").resized(Scene::Size());

	texture.draw(texture_1_pos);
	texture.draw(texture_2_pos);
	texture.draw(texture_3_pos);
}
