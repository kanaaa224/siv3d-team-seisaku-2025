# include "Game.hpp"

# include "GameUI.hpp"
# include "../Objects/Player.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	// Playerの初期位置設定
	player = new Player(Vec2(100.0, 100.0));
	player->initialize();
}

void Game::update()
{
	if (MouseL.down()) changeScene(SceneState::Ranking);

	player->update();
}

void Game::draw() const
{
	ClearPrint();
	player->draw();
}
