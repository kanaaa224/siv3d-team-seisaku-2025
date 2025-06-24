# include <Siv3D.hpp>
# include "PlayerHUD.hpp"
# include "../Utils/CustomImageLoader.hpp"


PlayerHUD* PlayerHUD::instance = nullptr;

PlayerHUD::PlayerHUD()
	: flame_location(0.0, 0.0)
	, hp_location(0.0, 0.0)
	, fontBitmap{ 48 }
	, player_hp(100)
	, GO(false)
	, animation_number(0)
	, animation_time(0.0)
	, max_animation_time(30.0)
	, player_game_world_start_x(0.0)
	, player_game_world_end_x(0.0)
	, player_pos(0.0, 0.0)
{
	initialize();
}

void PlayerHUD::initialize()
{
	flame_location = { 30.0, 125.0 };
	hp_location = { 195.0, 22.0 };

	frameNames = {
		U"Attack_Frame",
		U"Movement_Frame"
	};

	iconNames = {
		U"Buf_Attack",
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
	elapsedTime = 0.0;

	//アニメーション用にロード 
	run_animation = LoadDivGraph(U"P_player_UI", Size(288, 45));

	//アニメーションの初期状態を設定
	animation_number = 0;
	animation_time = 0.0;
}


void PlayerHUD::update()
{
	//バフの処理
	if (KeyP.down())
	{
		buff_amount++;
	}

	//タイマーの開始処理
	if (!GO && player_state != 0)
	{
		GO = true;
	}
	if (GO && !timerStarted)
	{
		timerStarted = true;
		startTime = Scene::Time(); //タイマーの開始時間を記録
	}

	// タイマーが開始されていた場合の経過時間更新とUIアニメーションフレームの更新
	if (timerStarted)
	{
		m_elapsedTime = Scene::Time() - startTime;

		//進捗度UIのアニメーションフレームを更新
		animation_time += Scene::DeltaTime();
		const double frame_duration = 0.1; //各フレームを表示する時間（秒）

		if (animation_time >= frame_duration)
		{
			if (!run_animation.isEmpty())
			{
				animation_number = (animation_number + 1) % run_animation.size();
			}
			else
			{
				animation_number = 0; //アニメーションフレームがない場合は0にリセット
			}
			animation_time = 0.0; //フレーム時間が経過したらリセット
		}

		elapsedTime = Scene::Time() - startTime;
	}
	else
	{
		//タイマーが開始されていない場合はアニメーションを停止し、最初のフレームに戻す
		animation_number = 0;
		animation_time = 0.0;
	}

	if (boss_state == VaillantState::Death)
	{
		FileSave(elapsedTime);
	}
}

void PlayerHUD::draw() const
{
	// 画像を滑らかに補完
	const ScopedRenderStates2D rs{ SamplerState::ClampNearest };

	//進捗度UIの描画
	//ボス地点のUI
	TextureAsset(U"P_ishi_silver_UI").resized(80, 40).drawAt(950, 110);
	TextureAsset(U"P_boss_UI").resized(90, 92).drawAt(950, 65);
	TextureAsset(U"P_BOSS_UI").resized(45, 20).drawAt(946, 80);

	//スタート地点のUI
	TextureAsset(U"P_ishi_UI").resized(70, 40).drawAt(420, 100);

	//間の丸いやつ
	{
		Vec2 start = Vec2(445.0, 85.0);
		int space = 20;

		for (int i = 1; i < 23; i++)
		{
			Vec2 pos = start + Vec2{ space * i, 0 };
			TextureAsset(U"P_sphere_black_UI").resized(10, 10).drawAt(pos);
		}
	}

	//P_player_UIのアニメーション描画
	double progress_rate = 0.0;
	//プレイヤーの現在のX座標を player_pos から取得
	const double player_current_x = player_pos.x;

	//進捗度を計算
	if (player_game_world_end_x > player_game_world_start_x)
	{
		progress_rate = Clamp((player_current_x - player_game_world_start_x) / (player_game_world_end_x - player_game_world_start_x), 0.0, 1.0);
	}

	// プレイヤーUIの開始位置と終了位置を定義 (進捗度UI上の表示位置)
	Vec2 player_ui_start_pos = { 420.0, 70.0 };//P_base_UI の中心X座標に対応
	Vec2 player_ui_end_pos = { 950.0, 65.0 };//P_boss_UI の中心X座標に対応

	// 線形補間により、現在の進行度に応じたプレイヤーUIの位置を計算
	Vec2 player_current_pos = player_ui_start_pos.lerp(player_ui_end_pos, progress_rate);

	if (!run_animation.isEmpty())
	{
		run_animation[animation_number].resized(576, 90).drawAt(player_current_pos);
	}

	//その他のUI要素の描画
	Vec2 position{ 40, 40 };
	Vec2 size{ 110, 110 };
	Vec2 HP_size{ 200, 35 };

	const int max_hp = 100;
	double full_width = HP_size.x - 25;

	double PlayerMaxHP = 100.0;
	double PlayerHP = player_hp;
	double gaugeWidth = static_cast<double>(PlayerHP) / PlayerMaxHP * 180;

	//キャラクターフレームとアイコン
	TextureAsset(U"Character Frame").resized(size).drawAt(position);
	TextureAsset(U"Character Icon").resized(size.x - 10, size.y - 10).drawAt(position);

	//HPフレームとHPバー
	TextureAsset(U"HP_frame").resized(HP_size).drawAt(hp_location);
	Vec2 bar_size{ full_width, HP_size.y - 25 };
	Vec2 bar_top_left = hp_location - Vec2{ bar_size.x / 2.0, bar_size.y / 2.0 };
	TextureAsset(U"HP_bar").resized(gaugeWidth, bar_size.y).draw(bar_top_left);

	//タイムフレームと表示
	TextureAsset(U"time_frame").resized(200, 50).drawAt(195, 70);
	if (timerStarted)
	{
		const Font& timeFont = FontAsset(U"TitleFont");
		//double elapsedTime = Scene::Time() - startTime;
		timeFont(U"{:.2f}"_fmt(elapsedTime)).drawAt(30, Vec2{ 195, 70 }, ColorF{ 0.1, 0.1, 0.1 });
		FontAsset(U"TitleFont")(U"{:.2f}"_fmt(elapsedTime)).drawAt(TextStyle::
			OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 200, 70 });
	}
	else
	{
		FontAsset(U"TitleFont")(U"--:--").drawAt(TextStyle::
			OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 200, 70 });
	}

	//バフのアイコンとフレーム表示
	Vec2 start = flame_location;
	int spacing = 150;
	for (size_t i = 0; i < frameNames.size(); ++i)
	{
		Vec2 pos = start + Vec2{ spacing * i, 0 };
		int a = 40;
		TextureAsset(frameNames[i]).resized(a, a).drawAt(pos);
		TextureAsset(iconNames[i]).resized(a, a).drawAt(pos);
	}
	fontBitmap(U"×" + Format(buff_amount)).draw(25, Vec2{ 10, 50 });

	//プレイヤーの操作説明UI
	if (player_vel.y == 0.0) //地面にいる場合
	{
		FontAsset(U"TitleFont")(U"攻撃：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 1110, 40 });
		xbutton.resized(35, 35).drawAt(1205, 45);
		FontAsset(U"TitleFont")(U"回避攻撃：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 1130, 80 });
		Bbutton.resized(35, 35).drawAt(1205, 85);
		FontAsset(U"TitleFont")(U"ジャンプ：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 1130, 120 });
		Abutton.resized(35, 35).drawAt(1206, 125);
	}
	else //空中にいる場合
	{
		FontAsset(U"TitleFont")(U"ジャンプ攻撃：").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 1110, 40 });
		xbutton.resized(35, 35).drawAt(1210, 45);
		Bbutton.resized(35, 35).drawAt(1245, 45);
	}

	//回避ボタンのクールタイム表示
	if (player_avoid > 0.0)
	{
		Avoid_button_frame.resized(50, 50).drawAt(340, 75);
		Avoid_button_image.resized(55, 55).drawAt(340, 75, ColorF{ 0.5, 0.5, 0.5 }); //クールタイム中→暗く表示
	}
	else
	{
		Avoid_button_frame.resized(50, 50).drawAt(340, 70);
		Avoid_button_image.resized(55, 55).drawAt(340, 70); //クールタイム中ではない→通常表示
	}

	//ゲームクリアまたはゲームオーバーの表示
	if (boss_erea_flg == true && boss_state == VaillantState::Death)
	{
		FontAsset(U"TitleFont")(U"Game Clear").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 200, Vec2{ 640, 200 });
	}
	else if (player_state == ePlayerState::die)
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

void PlayerHUD::FileSave(double ClearTime)
{
	//書き込み用のテキストファイルをオープンする
	TextWriter wirter{ U"../assets/text/ClearTime.txt" };

	//例外スロー確認
	if (!wirter)
	{
		throw Error{ U"Failed to open `ClearTime.txt`" };
	}

	//1行書き込み(改行なし)
	wirter.write(Format(ClearTime));
	wirter.close();
}

void PlayerHUD::resetTime()
{
	m_elapsedTime = 0.0;

	//タイマー関連の変数を初期化
	timerStarted = false;
	startTime = 0.0;
	GO = false;
	m_elapsedTime = 0.0;
	animation_number = 0; //アニメーションを最初のフレームにリセット
	animation_time = 0.0; //アニメーション時間をリセット
	player_pos = Vec2(0.0, 0.0); //プレイヤー位置もリセット (必要であれば)
	elapsedTime = 0.0;
}
