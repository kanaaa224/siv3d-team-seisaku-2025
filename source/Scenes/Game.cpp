# include "Game.hpp"
# include "../Stages/1.hpp"
# include "../Stages/DebugBoss.hpp"

Game::Game(const InitData& init) : IScene{ init }
{
	if (Key1.pressed())
	{
		StageDebugBoss::NewInstance();
		return;
	}

	switch (getData().current_stage)
	{
	case 0:
		Stage::NewInstance();
		break;

	case 1:
		Stage1::NewInstance();
		break;

	default:
		break;
	}
}

void Game::update()
{
	Stage* stage = Stage::GetInstance();

	stage->update();

	if (Key0.down()) changeScene(SceneState::Title, 0.5s);
}

void Game::draw() const
{
	Stage* stage = Stage::GetInstance();

	stage->draw();
}
