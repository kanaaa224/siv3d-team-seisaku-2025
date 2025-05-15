# include "Game.hpp"

# include "GameUI.hpp"
# include "../Objects/Player.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	// Playerの初期位置設定
	player = new Player(Vec2(50, 640.0));
	player->initialize();

	//UIのインスタンスを取得
	GameUI* gameui = GameUI::GetInstance();
	//イニシャライズ呼び出し
	gameui->initialize();
}

void Game::update()
{
	//UIのインスタンスを取得
	GameUI* gameui = GameUI::GetInstance();

	if (MouseL.down()) changeScene(SceneState::Ranking);

	player->update();

	//更新処理呼び出し
	gameui->update();

}

void Game::draw() const
{
	//ClearDrawScreenと一緒
	ClearPrint();
	player->draw();

	//UIのインスタンスを取得
	GameUI* gameui = GameUI::GetInstance();
	//描画処理呼び出し
	gameui->draw();
}
