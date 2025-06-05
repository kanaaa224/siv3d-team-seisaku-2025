# include "StageBackground.hpp"

StageBackground::StageBackground(P2World& world, const Vec2& position) : ObjectBase(world, position) {}

void StageBackground::draw() const
{
	const auto texture = TextureAsset(U"Stage 1 Background").resized(Scene::Size());

	const int width = Scene::Width();
	const int baseX = static_cast<int>(std::floor(camera_pos.x / width)) * width;

	for (int i = -1; i <= 2; ++i)
	{
		texture.draw(baseX + i * width, 0);
	}
}
