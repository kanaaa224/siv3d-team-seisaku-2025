# include "StageBackground.hpp"

StageBackground::StageBackground(const Vec2& start_position): ObjectBase(start_position)
{
	this->initialize();
}

StageBackground::~StageBackground()
{
	this->finalize();
}

void StageBackground::initialize()
{

}

void StageBackground::update()
{

}

void StageBackground::draw() const
{
	TextureAsset(U"Stage 1 Background").resized(Scene::Size()).draw(position); // 仮
}

void StageBackground::finalize()
{

}
