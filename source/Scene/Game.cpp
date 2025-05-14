# include "Game.hpp"

# include "GameUI.hpp"
# include "../Objects/Player.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{

}

void Game::update()
{
	if (MouseL.down()) changeScene(SceneState::Ranking);
}

void Game::draw() const
{

}
