﻿# include "Stage1.hpp"

# include "../Objects/GameUI.hpp"
# include "../Objects/Player.hpp"
# include "../Objects/StageBackground.hpp"
# include  "../Objects/Enemy/Scarerun/Scarerun.hpp"

Stage1::Stage1()
{
	this->initialize();
}

void Stage1::initialize()
{
	createObject<StageBackground>(Vec2{ 0, 0 });
	createObject<Scarerun>(Vec2{ 450, 500 });
	createObject<Player>(Vec2{ 320, 500 });

	floor = world.createRect(P2Static, Vec2{ 640, 600 }, SizeF{ 1000, 10 });
}

void Stage1::update()
{
	Stage::update();

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->update();
}

void Stage1::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

	Stage::draw();

	floor.draw();

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->draw();
}

void Stage1::NewInstance()
{
	if (instance != nullptr) delete instance;

	instance = new Stage1();
}
