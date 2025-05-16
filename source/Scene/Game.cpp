# include "Game.hpp"
# include "../Objects/Stage.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{

}

void Game::update()
{
	Stage* stage = Stage::GetInstance();

	stage->update();

	if (MouseL.down()) changeScene(SceneState::Title, 0.5s);
}

void Game::draw() const
{
	Stage* stage = Stage::GetInstance();

	stage->draw();
}
