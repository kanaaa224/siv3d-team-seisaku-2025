#include "Stage.hpp"

Stage::Stage(const Vec2& start_position): ObjectBase(start_position)
{

}

Stage::~Stage()
{

}

void Stage::initialize()
{
	// アセットの登録
	TextureAsset::Register({ U"Stage1_background", { U"Stage" } }, U"../assets/images/stage/stage1/forest.png");
}

void Stage::update()
{

}

void Stage::draw() const
{
	TextureAsset(U"Stage1_background").resized(1280, 720).draw(0,0);
}

void Stage::finalize()
{

}
