# include <Siv3D.hpp>
# include "PlayerHUD.hpp"
# include "../Utils/CustomImageLoader.hpp"


PlayerHUD* PlayerHUD::instance = nullptr;

PlayerHUD::PlayerHUD() :flame_location(0.0, 0.0), hp_location(0.0, 0.0), fontBitmap{ 48 }, player_hp(100),GO(false)
{
	initialize();
}

void PlayerHUD::initialize()
{

	flame_location = { 20.0, 125.0 };

	hp_location = { 195.0,22.0 };

	frameNames = {
		U"Attack_Frame",
		//U"Defense_Frame",
		//U"Hpup_Frame",
		//U"Recovery_Frame",
		U"Movement_Frame"
	};

	iconNames = {
		U"Buf_Attack",
		//U"Buf_defense",
		//U"Buf_Hpup",
		//U"Buf_recovery",
		U"Buf_Movement"
	};

	buff_amount = 0;

	Avoid_button_image = TextureAsset(U"Buf_Movement");
	Avoid_button_frame = TextureAsset(U"avoidance_button");

	button = LoadDivGraph(U"Button", Size(16, 16));

	xbutton = button[0];
	Abutton = button[5];
	Bbutton = button[15];

	//タイマー関連の変数を初期化
	timerStarted = false;
	startTime = 0.0;
	GO = false;
	m_elapsedTime = 0.0;

}


void PlayerHUD::update()
{
	//バフ
	if (KeyP.down())
	{
		buff_amount++;
	}

	//タイマー用
	if (!GO && player_state != 0)
	{
		GO = true;
	}
	if (GO && !timerStarted)
	{
		timerStarted = true;
		startTime = Scene::Time(); // タイマーの開始時間を記録
	}
	if (timerStarted)
	{
		m_elapsedTime = Scene::Time() - startTime;
	}
}

void PlayerHUD::draw() const
{
	Vec2 position{ 40, 40 };

	Vec2 size{ 110, 110 };

	Vec2 HP_size{ 200,35 };

	// HPバー関係の定義
	const int max_hp = 100;
	//double hp_rate = Clamp(static_cast<double>(player_hp) / max_hp, 0.0, 1.0); // 0～1に制限

	double full_width = HP_size.x - 25;
	double hp_width = 180.0;

	double PlayerMaxHP = 100.0;
	double PlayerHP = player_hp;

	double gaugeWidth = static_cast<double>(PlayerHP) / PlayerMaxHP * 180;

	// resizedで画像の描画サイズ(拡大率？) drawAtで中心座標を元に描画位置を設定(描画したい座標を設定)
	// 変数を使ってるのはframeの中にiconを入れたいため
	TextureAsset(U"Character Frame").resized(size).drawAt(position);
	TextureAsset(U"Character Icon").resized(size.x - 10, size.y - 10).drawAt(position);

	TextureAsset(U"HP_frame").resized(HP_size).drawAt(hp_location);
	//TextureAsset(U"HP_bar")(0, 0, 0, 0).resized(HP_size.x - 25, HP_size.y - 25).drawAt(hp_location);

// バーの描画左上座標（左端を固定）したい
	Vec2 bar_size{ full_width, HP_size.y - 25 };
	Vec2 bar_top_left = hp_location - Vec2{ bar_size.x / 2.0, bar_size.y / 2.0 };

	//HPゲージの中身
	TextureAsset(U"HP_bar").resized(gaugeWidth, bar_size.y).draw(bar_top_left);

	TextureAsset(U"time_frame").resized(200, 50).drawAt(195, 70);

	// タイマーが開始されていたら時間を表示させｒ
	if (timerStarted)
	{
		const Font& timeFont = FontAsset(U"TitleFont");
		double elapsedTime = Scene::Time() - startTime; // 経過時間を計算

		timeFont(U"{:.2f}"_fmt(elapsedTime)).drawAt(30, Vec2{ 195, 70 }, ColorF{ 0.1, 0.1, 0.1 });
		FontAsset(U"TitleFont")(U"{:.2f}"_fmt(elapsedTime)).drawAt(TextStyle::
			OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 200, 70 });
	}
	else
	{
		// タイマーが開始されていない場合の表示
		FontAsset(U"TitleFont")(U"--:--").drawAt(TextStyle::
			OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 200, 70 });
	}

	// 読み込みたい画像のパスをmain.cppに書きます。
	// 書いた「U"?? ??"」が名前です。
	// これをTextureAsset(U"?? ??")の中に名前を入れないと呼び出しても描画されません。
	// .resized()は 0 0 以外の値を入れてください ちなみに画像のサイズです。
	// .drawAt()は描画したい画像の中心座標になります。


	//バフのアイコンとフレーム表示
	Vec2 start = flame_location; //描画の開始位置　左側
	int spacing = 150;            //横間隔

	for (size_t i = 0; i < frameNames.size(); ++i)
	{
		Vec2 pos = start + Vec2{ spacing * i, 0 };//X方向にずらしたい

		int a = 40;

		TextureAsset(frameNames[i]).resized(a, a).drawAt(pos);
		TextureAsset(iconNames[i]).resized(a, a).drawAt(pos);
	}

	fontBitmap(U"×" + Format(buff_amount)).draw(25, Vec2{ 10, 50 });



	// プレイヤーが地面にいる場合
	if (player_vel.y == 0.0)
	{
		FontAsset(U"TitleFont")(U"攻撃：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 1110, 40 });
		xbutton.resized(35, 35).drawAt(1205, 45);
		FontAsset(U"TitleFont")(U"回避攻撃：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 1130, 80 });
		Bbutton.resized(35, 35).drawAt(1205, 85);
		FontAsset(U"TitleFont")(U"ジャンプ：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 1130, 120 });
		Abutton.resized(35, 35).drawAt(1206, 125);
		
	}
	// プレイヤーが地面にいない場合（ジャンプ中など）
	else
	{
		FontAsset(U"TitleFont")(U"ジャンプ攻撃：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 1110, 40 });
		xbutton.resized(35, 35).drawAt(1210, 45);
		Bbutton.resized(35, 35).drawAt(1245, 45);
	}


	if (player_avoid > 0.0)
	{
		Avoid_button_frame.resized(50, 50).drawAt(340, 75);
		// クールタイム中→暗く表示
		Avoid_button_image.resized(55, 55).drawAt(340, 75, ColorF{ 0.5, 0.5, 0.5 });

	}
	else
	{
		Avoid_button_frame.resized(50, 50).drawAt(340, 70);
		// クールタイム中ではない→通常表示
		Avoid_button_image.resized(55, 55).drawAt(340, 70);
	}

	// GameClear or Over 表示
	if (boss_erea_flg == true && boss_state == VaillantState::Death)
	{
		FontAsset(U"TitleFont")(U"Game Clear").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 200, Vec2{ 640, 200 });
	}
	else if(player_state == ePlayerState::die)
	{
		FontAsset(U"TitleFont")(U"Game Over").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 200, Vec2{ 640, 200 });
	}

#ifdef _DEBUG

#endif


}

PlayerHUD* PlayerHUD::GetInstance()
{
	if (instance == nullptr) instance = new PlayerHUD();

	return instance;
}


void PlayerHUD::resetTime()
{
	m_elapsedTime = 0.0;

	//タイマー関連の変数を初期化
	timerStarted = false;
	startTime = 0.0;
	GO = false;
	m_elapsedTime = 0.0;
}
