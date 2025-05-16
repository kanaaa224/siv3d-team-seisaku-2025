# include "Stage.hpp"

# include "GameUI.hpp"
# include "Player.hpp"
# include "StageBackground.hpp"

Stage* Stage::instance = nullptr;

Stage::Stage()
{
	this->initialize();
}

Stage::~Stage()
{
	this->finalize();
}

void Stage::initialize()
{
	objects << new StageBackground(Vec2());
	objects << new Player(Vec2(50, 640));
}

void Stage::update()
{
	for (const auto& object : objects)
	{
		object->update();
	}

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->update();
}

void Stage::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

	for (const auto& object : objects)
	{
		object->draw();
	}

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->draw();
}

void Stage::finalize()
{

}

Stage* Stage::GetInstance()
{
	if (instance == nullptr) instance = new Stage();

	return instance;
}
