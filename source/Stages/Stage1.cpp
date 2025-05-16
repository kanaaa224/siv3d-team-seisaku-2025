# include "Stage1.hpp"

# include "../Objects/GameUI.hpp"
# include "../Objects/Player.hpp"
# include "../Objects/StageBackground.hpp"
#include  "../Objects/Enemy/Scarerun/Scarerun.hpp"

Stage1* Stage1::instance = nullptr;

Stage1::Stage1()
{
	this->initialize();
}

Stage1::~Stage1()
{
	this->finalize();
}

void Stage1::initialize()
{
	objects << new StageBackground(Vec2());
	objects << new Player(Vec2(50, 640));
	objects << new Scarerun(Vec2(300, 640));
}

void Stage1::update()
{
	for (const auto& object : objects)
	{
		object->update();
	}

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->update();
}

void Stage1::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

	for (const auto& object : objects)
	{
		object->draw();
	}

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->draw();
}

void Stage1::finalize()
{

}

Stage1* Stage1::GetInstance()
{
	if (instance == nullptr) instance = new Stage1();

	return instance;
}
