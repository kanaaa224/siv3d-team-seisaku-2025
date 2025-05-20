# include "GameUI.hpp"

GameUI* GameUI::instance = nullptr;

GameUI::GameUI():flame_location(0.0, 0.0) , hp_location(0.0, 0.0), fontBitmap{ 48 }
{
	this->initialize();
}

GameUI::~GameUI()
{
	this->finalize();
}

void GameUI::initialize()
{
	flame_location = {350.0, 55.0 };

	hp_location = { 195.0,22.0 };

	frameNames = {
		U"Attack_Frame",
		U"Defense_Frame",
		U"Hpup_Frame",
		U"Recovery_Frame",
		U"Movement_Frame"
	};

	iconNames = {
		U"Buf_Attack",
		U"Buf_defense",
		U"Buf_Hpup",
		U"Buf_recovery",
		U"Buf_Movement"
	};
}

void GameUI::update()
{

}

void GameUI::draw() const
{
	Vec2 position{ 40, 40 };

	Vec2 size{ 110, 110 };

	Vec2 HP_size{ 200,35 };

	// resizedで画像の描画サイズ(拡大率？) drawAtで中心座標を元に描画位置を設定(描画したい座標を設定)
	// 変数を使ってるのはframeの中にiconを入れたいため
	TextureAsset(U"Character Frame").resized(size).drawAt(position);
	TextureAsset(U"Character Icon").resized(size.x - 10, size.y - 10).drawAt(position);

	TextureAsset(U"HP_frame").resized(HP_size).drawAt(hp_location);
	TextureAsset(U"HP_bar").resized(HP_size.x -25,HP_size.y-25).drawAt(hp_location);

	TextureAsset(U"time_frame").resized(200,50).drawAt(195,70);


	// 読み込みたい画像のパスをmain.cppに書きます。
	// 書いた「U"?? ??"」が名前です。
	// これをTextureAsset(U"?? ??")の中に名前を入れないと呼び出しても描画されません。
	// .resized()は 0 0 以外の値を入れてください ちなみに画像のサイズです。
	// .drawAt()は描画したい画像の中心座標になります。
	
	Vec2 start = flame_location; //描画の開始位置　左側
	int spacing = 180;            //横間隔

	for (size_t i = 0; i < frameNames.size(); ++i)
	{
		Vec2 pos = start + Vec2{ spacing * i, 0 };//X方向にずらしたい

		TextureAsset(frameNames[i]).resized(50, 50).drawAt(pos);
		TextureAsset(iconNames[i]).resized(50, 50).drawAt(pos);
	}

	fontBitmap(U"×00").draw(35, Vec2{ 560, 30 });
}

void GameUI::finalize()
{

}

GameUI* GameUI::GetInstance()
{
	if (instance == nullptr) instance = new GameUI();

	return instance;
}
