# include "GameUI.hpp"

GameUI* GameUI::instance = nullptr;

GameUI::GameUI():flame_location(0.0, 0.0)
{
	this->initialize();
}

GameUI::~GameUI()
{
	this->finalize();
}

void GameUI::initialize()
{
	flame_location = { 600.0, 50.0 };
}

void GameUI::update()
{

}

void GameUI::draw() const
{
	Vec2 position{ 40, 40 };

	Vec2 size{ 110, 110 };

	// resizedで画像の描画サイズ(拡大率？) drawAtで中心座標を元に描画位置を設定(描画したい座標を設定)
	// 変数を使ってるのはframeの中にiconを入れたいため
	TextureAsset(U"Character Frame").resized(size).drawAt(position);
	TextureAsset(U"Character Icon").resized(size.x - 10, size.y - 10).drawAt(position);

	// 読み込みたい画像のパスをmain.cppに書きます。
	// 書いた「U"?? ??"」が名前です。
	// これをTextureAsset(U"?? ??")の中に名前を入れないと呼び出しても描画されません。
	// .resized()は 0 0 以外の値を入れてください ちなみに画像のサイズです。
	// .drawAt()は描画したい画像の中心座標になります。
	TextureAsset(U"Attack Frame").resized(50, 50).drawAt(flame_location);
}

void GameUI::finalize()
{

}

GameUI* GameUI::GetInstance()
{
	if (instance == nullptr) instance = new GameUI();

	return instance;
}
