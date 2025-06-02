# include "PlayerHUD.hpp"
# include "../Utils/CustomImageLoader.hpp"


PlayerHUD* PlayerHUD::instance = nullptr;

PlayerHUD::PlayerHUD() :flame_location(0.0, 0.0), hp_location(0.0, 0.0), fontBitmap{ 48 }, player_hp(100)
{
	initialize();
}

void PlayerHUD::initialize()
{
	flame_location = { 350.0, 55.0 };

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

	buff_amount = 0;

	button = LoadDivGraph(U"Button", Size(16, 16));

	xbutton = button[0];
	Abutton = button[5];
	Bbutton = button[15];

}

void PlayerHUD::update()
{
	if (KeyP.down())
	{
		buff_amount++;
	}
}

void PlayerHUD::draw() const
{

	Print << U"Player HP: " << player_hp;

	Vec2 position{ 40, 40 };

	Vec2 size{ 110, 110 };

	Vec2 HP_size{ 200,35 };

	// HPバー関係の定義
	const int max_hp = 100;
	double hp_rate = Clamp(static_cast<double>(player_hp) / max_hp, 0.0, 1.0); // 0～1に制限

	double full_width = HP_size.x - 25;
	double hp_width = (HP_size.x - 25) * hp_rate;

	// resizedで画像の描画サイズ(拡大率？) drawAtで中心座標を元に描画位置を設定(描画したい座標を設定)
	// 変数を使ってるのはframeの中にiconを入れたいため
	TextureAsset(U"Character Frame").resized(size).drawAt(position);
	TextureAsset(U"Character Icon").resized(size.x - 10, size.y - 10).drawAt(position);

	TextureAsset(U"HP_frame").resized(HP_size).drawAt(hp_location);
	//TextureAsset(U"HP_bar")(0, 0, 0, 0).resized(HP_size.x - 25, HP_size.y - 25).drawAt(hp_location);

// バーの描画左上座標（左端を固定）したい
	Vec2 bar_size{ full_width, HP_size.y - 25 };
	Vec2 bar_top_left = hp_location - Vec2{ bar_size.x / 2.0, bar_size.y / 2.0 };

	TextureAsset(U"HP_bar")(0, 0, static_cast<int>(hp_width), static_cast<int>(bar_size.y))
		.resized(hp_width, bar_size.y)
		.draw(bar_top_left);

	//if (hp_rate >= 1.0)
	//{
	//	// HP満タン → 画像そのまま描画させる
	//	TextureAsset(U"HP_bar").resized(bar_size).draw(bar_top_left);
	//}
	//else if (hp_rate > 0.0)
	//{
	//	int cut_width = static_cast<int>(hp_width);

	//	// ひだりこてい
	//	//TextureAsset(U"HP_bar")(0, 0, cut_width, static_cast<int>(bar_size.y)).resized(hp_width, bar_size.y).draw(bar_top_left);
	//	TextureAsset(U"HP_bar").resized(100, bar_size.y).draw(bar_top_left);
	//}




	TextureAsset(U"time_frame").resized(200, 50).drawAt(195, 70);

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

	fontBitmap(U"×" + Format(buff_amount)).draw(35, Vec2{ 560, 30 });



	// プレイヤーが地面にいる場合
	if (player_vel.y == 0.0)
	{
		FontAsset(U"TitleFont")(U"攻撃ボタン：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 100, 110 });
		xbutton.resized(35, 35).drawAt(210, 115);
		FontAsset(U"TitleFont")(U"ジャンプボタン：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 120, 150 });
		Abutton.resized(35, 35).drawAt(260, 155);
		FontAsset(U"TitleFont")(U"回避ボタン：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 100, 190 });
		Bbutton.resized(35, 35).drawAt(210, 195);
	}
	// プレイヤーが地面にいない場合（ジャンプ中など）
	else
	{
		FontAsset(U"TitleFont")(U"ジャンプ攻撃ボタン：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 130, 110 });
		xbutton.resized(35, 35).drawAt(270, 115);
	}

}

PlayerHUD* PlayerHUD::GetInstance()
{
	if (instance == nullptr) instance = new PlayerHUD();

	return instance;
}
