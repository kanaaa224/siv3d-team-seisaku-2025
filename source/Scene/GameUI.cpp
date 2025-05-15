#include "GameUI.hpp"

// 静的メンバ変数定義 nullptrで初期化
GameUI* GameUI::instance = nullptr;

GameUI::GameUI():
	position(0.0, 0.0)
{
}

GameUI::~GameUI()
{

}

void GameUI::initialize()
{
	position = { 300.0, 300.0 };
}

void GameUI::update()
{

}

void GameUI::draw()const
{
	// resizedで画像の描画サイズ(拡大率？) drawAtで中心座標を元に描画位置を設定(描画したい座標を設定)
	// 変数を使ってるのはframeの中にiconを入れたいため
	TextureAsset(U"Character_frame").resized(26 * 2, 24 * 2).drawAt(position);
	TextureAsset(U"Character_icon").fitted(26 * 2, 24 * 2).drawAt(position);
}

void GameUI::finalize()
{

}

GameUI* GameUI::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameUI();
	}
	return instance;
}
