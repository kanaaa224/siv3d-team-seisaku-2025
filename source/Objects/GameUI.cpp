# include "GameUI.hpp"

GameUI* GameUI::instance = nullptr;

GameUI::GameUI()
{
	this->initialize();
}

GameUI::~GameUI()
{
	this->finalize();
}

void GameUI::initialize()
{

}

void GameUI::update()
{

}

void GameUI::draw() const
{
	Vec2 position{ 0, 0 };

	// resizedで画像の描画サイズ(拡大率？) drawAtで中心座標を元に描画位置を設定(描画したい座標を設定)
	// 変数を使ってるのはframeの中にiconを入れたいため
	TextureAsset(U"Character Frame").resized(26 * 2, 24 * 2).draw(position);
	TextureAsset(U"Character Icon").fitted(26 * 2, 24 * 2).draw(position);
}

void GameUI::finalize()
{

}

GameUI* GameUI::GetInstance()
{
	if (instance == nullptr) instance = new GameUI();

	return instance;
}
