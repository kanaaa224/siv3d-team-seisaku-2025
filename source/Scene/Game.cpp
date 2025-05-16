# include "Game.hpp"
# include "GameUI.hpp"

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

	stage = new Stage(Vec2(1280.0, 720.0));
	stage->initialize();
}

Game::~Game()
{
	delete player;
	delete stage;
}

void Game::update()
{
	//UIのインスタンスを取得
	GameUI* gameui = GameUI::GetInstance();

	if (MouseL.down()) changeScene(SceneState::Title, 0.5s);

	player->update();

	//更新処理呼び出し
	gameui->update();

}

void Game::draw() const
{
	stage->draw();

	//ClearDrawScreenと一緒
	ClearPrint();
	player->draw();

	//UIのインスタンスを取得
	GameUI* gameui = GameUI::GetInstance();
	//描画処理呼び出し
	gameui->draw();
}
