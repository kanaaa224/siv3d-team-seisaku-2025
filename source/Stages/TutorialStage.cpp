# include "TutorialStage.hpp"
# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Objects/HitBox.hpp"
# include "../Characters/Player.hpp"
# include "../UI/PlayerHUD.hpp"
# include "../Characters/Enemies/Scarerun/Scarerun.hpp"
# include "../Utils/CustomImageLoader.hpp"
#include <random>

#define SIZE 20	//文字のサイズ
#define DRAW_POSITION Vec2{Scene::Width() / 2 - 445.0, Scene::Height() / 2 - 50.0} //座標
#define HELP_TIMER 7.0

TutorialStage::TutorialStage()
{
	m_currentIndex = 0;

	AudioAsset(U"tutorial1_BGM").setVolume(1.0);
	//AudioAsset(U"Battle_BGM").stop();
	AudioAsset(U"tutorial1_BGM").play();

	// ステージの開始・終了ワールド座標を設定
	//プレイヤーの初期位を回質店
	m_stageStartX = 100.0;
	//Bossの生成位置を終了点
	m_stageEndX = (Scene::Width() / 2) + 4900.0;

	starting_point = Vec2(Scene::Width() / 2 - 445.0, Scene::Height() / 2 - 50.0);
	last_point = Vec2(Scene::Width() / 2 - 445.0/*1085.0*/, Scene::Height() / 2 - 50.0);
	gaugeStarted = false;
	gaugeStartTime = 0.0;

	Loding_image = TextureAsset(U"Loading_image");

	run_animation = LoadDivGraph(U"Player Run", Size(288, 45));
	runAnimationFrame = 0;
	runAnimationTime = 0.0;
	transition = false;
	glowPos = Vec2(Scene::Width() / 2, Scene::Height() / 2);
	textJustSwitched = false;

	initialize();
}

void TutorialStage::initialize()
{
	// --- ロード画面表示 ---
	Scene::SetBackground(ColorF{ 0.0 }); // 背景を黒に設定
	//Loding_image.resized(Size(1500, 500)).draw(-40, 30);
	//FontAsset(U"TitleFont")(U"Loading...").drawAt(Scene::Center(), Palette::White);
	
	Scene::SetBackground(ColorF{ 0.0 });

	//ランダムメッセージ用
	Font messageFont(50);

	
	Font loadingFont(30);

	Array<String> messages = {
		U"キノコからのドロップ品は攻撃力UP！",
		U"コウモリからのドロップ品はスピードUP！",
		U"コウモリにはジャンプ攻撃が有効だ！",
	};

	String randomMessage = messages.choice();

	const double startTime = Scene::Time();
	const double duration = 2.0; // 2秒で1ループ
	bool shownOnce = false;

	while (System::Update())
	{
		Scene::SetBackground(ColorF(0.1));


		double elapsed = Scene::Time() - startTime;
		int dotCount = static_cast<int>(elapsed * 3) % 4; // 0,1,2,3

		String dots = U"";
		for (int i = 0; i < dotCount; ++i)
		{
			dots += U".";
		}

		//ランダムにメッセージ
		messageFont(randomMessage).drawAt(Scene::Center(), ColorF{117/255.0,184/255.0,128/255.0});

		//アニメーションさえるやつ
		const Vec2 basePos = Vec2{ Scene::Width() - 260, Scene::Height() - 60 };
		loadingFont(U"Now Loading").draw(basePos, Palette::White);
		loadingFont(dots).draw(basePos + Vec2{ 185, 0 }, Palette::White);


		//抜ける
		if (elapsed > 2.5 && !shownOnce)
		{
			shownOnce = true;
			break;
		}
	}


	System::Update(); // 1フレーム強制描画

	//ステージオブジェクト
	createObject<StageBackground>();
	createObject<Ground>(Vec2{ Scene::Width(), (Scene::Height() + 5) });
	createObject<Wall>(Vec2{ -5, 500 });
	createObject<Wall>(Vec2{ Scene::Width(), 500});

	//プレイヤー
	createObject<Player>(Vec2{ 100, 650 });

	camera = Camera2D(Vec2{ (Scene::Width() / 2), (Scene::Height() / 2) }, 1.0, CameraControl::None_);

	////text
	helpTexts =
	{
		U"ようこそ『森の影』へ。\n敵を倒して強化し、森のボスを目指すアクションゲームです。",
		U"ボスを倒すことでステージクリアとなります。\nクリアタイムに応じてランクが表示されます。",
		U"左スティック or 十字キーを左右に倒すと、\nプレイヤーが移動します。\n実際に動いてみましょう。",
		U"Aボタンでジャンプできます。\n敵を飛び越えたり、攻撃を避けたりできます。",
		U"Xボタンで攻撃します。\n敵に接近して攻撃してみましょう。",
		U"Bボタンで回避攻撃ができます。\n移動しながら攻撃でき、敵をすり抜けることも可能です。\n使用後、再度使えるまで時間がかかります。",
		U"ジャンプ中にXボタンを押すと、空中攻撃ができます。\n空中の敵や高い位置にも攻撃が届きます。",
		U"ジャンプ中にBボタンを押すと、空中回避ができます。\n移動しながら攻撃でき、敵をすり抜けることも可能です。\n使用後、再度使えるまで時間がかかります。",
		U"敵を倒すと、バフアイテムが出現します。\n・赤いアイテム：攻撃力が上昇（DamageUp）\n・青いアイテム：移動速度が上昇（SpeedUp）\nふれると獲得できます。",
		U"画面左上：現在のHP(オレンジ色のゲージ) \n画面左上：ステージ開始時からの経過タイム \nステージクリア後、このタイムに応じてランクが表示されます。",
		U"右へ進むと、いよいよ本編が始まります。\nあなたの力で森の影に挑みましょう。"
	};

	for (const auto& section : helpTexts)
	{
		FontAsset(U"TitleFont")(section).draw(-9999, -9999); // ウォームアップ
	}

	isLoading = false; // 読み込み完了
	autoSkipTimer.start();
	
	//autoSkipTimer.start();

}

void TutorialStage::update()
{
	auto controller = XInput(0);

	Stage::update();

	static Stopwatch respawnTimer{ StartImmediately::No };

	bool enemySpawn = false;

	Player* player = nullptr;
	PlayerHUD* playerHUD = PlayerHUD::GetInstance();

	playerHUD->player_game_world_start_x = m_stageStartX;
	playerHUD->player_game_world_end_x = m_stageEndX;

	auto _objects_ = objects;

	for (const auto& object : objects)
	{
		if ((player = dynamic_cast<Player*>(object))) break;
	}

	if (player)
	{
		respawnTimer.reset();

		for (const auto& object : _objects_)
		{
			if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(object))
			{
				enemy->setPlayerPos(player->getBody().getPos());
			}

			if (dynamic_cast<Scarerun*>(object))
			{
				enemySpawn = true;
				break;
			}
		}

		for (const auto& object : _objects_)
		{
			if (HitBox* hitBox = dynamic_cast<HitBox*>(object))
			{
				hitBox->destroy();
			}

			if (Wall* wall = dynamic_cast<Wall*>(object))
			{
				if (transition)
				{
					wall->getBody().setPos(Vec2(Scene::Width() + 100.0, 500.0));
				}
			}
		}

		// 敵召喚
		if (!enemySpawn && currentTextIndex >= 3)
		{
			createObject<Scarerun>(Vec2{ 1000, Scene::Height() - 45 });
		}

		// Playerが動くとゲージスタート
		if (!gaugeStarted && player->getplayerstate() != ePlayerState::idle)
		{
			gaugeStarted = true;
			gaugeStartTime = Scene::Time();
		}

		playerHUD->setPlayerHP(player->getHP());
		playerHUD->update();
		playerHUD->setPlayerState(player->getplayerstate());
		playerHUD->setPlayerVel(player->getBody().getVelocity());
		playerHUD->setPlayeravoid(player->getAvoidanceCooldown());
		playerHUD->setPlayerPosition(player->getBody().getPos());

		//バフの数を取得、HUDに設定
		playerHUD->setBuffDamageUpCount(player->GetPlayerBuffDamageUpCount());
		playerHUD->setBuffSpeedUpCount(player->GetPlayerBuffSpeedUpCount());
	}
	else
	{
		if (!respawnTimer.isRunning()) respawnTimer.restart();

		if (respawnTimer.sF() >= 1.0)
		{
			createObject<Player>(Vec2{ (Scene::Width() / 2), 500 });

			respawnTimer.reset();
		}
	}

	// ゲージ更新
	if (gaugeStarted && sceneData().current_stage != 1 && !textJustSwitched)
	{
		last_point.x = getGaugePosition(195, 1085, HELP_TIMER, gaugeStartTime).x;
	}

	// 自動でテキストを切り替え
	if (last_point.x >= 1084 && !textJustSwitched)
	{
		textJustSwitched = true;
		// 最後のテキストだった場合はゲームシーンへ遷移
		if (currentTextIndex == helpTexts.size() - 1)
		{
			sceneData().current_stage = 1;
			last_point.x = 1085.0;
			transition = true;
		}
		else
		{
			++currentTextIndex;
			last_point.x = 195; // 次のゲージをリセット
			gaugeStartTime = Scene::Time(); // タイマー再スタート
			//autoSkipTimer.restart();
		}
	}

	// ゲージがリセットされたときにフラグ解除
	if (textJustSwitched && last_point.x <= 195 && !transition)
	{
		textJustSwitched = false;
	}

	// 遷移条件
	if (player->getBody().getPos().x >= 1290.0)
	{
		sceneChange(SceneState::Game, 2.0s);
		player->destroy();
	}

#ifdef _DEBUG
	//スキップ
	if (KeyEnter.down() || Gamepad(0).isConnected() && Gamepad(0).buttons[7].down())
	{
		sceneData().current_stage = 1;

		sceneChange(SceneState::Game, 0.5s);
	}
#endif
	//camera.update();
}

void TutorialStage::draw() const
{

	ClearPrint(); // 過去のPrint出力を消す
#ifdef _DEBUG

	Print << U"オブジェクト数: " << objects.size();
	Print << U"last_point : " << last_point;

#endif

	Stage::draw();

	if (transition)
	{
		const double glowAlpha = 0.5 + 0.5 * Periodic::Sine0_1(1.0); // 点滅 (0.5〜1.0)
		const int glowWidth = 70.0; // 右端から100pxの光

		// 画面右端に横グラデーション（左：透明 → 右：光る黄色）
		Rect{ Scene::Width() - glowWidth, 0, glowWidth, Scene::Height() }
			.draw(Arg::left = ColorF(1.0, 1.0, 0.9, 0.0),  // 左端：透明
				  Arg::right = ColorF(1.0, 1.0, 0.5, glowAlpha));  // 右端：点滅する黄色
	}

	//四角形描画
	RoundRect{ Arg::center(Scene::Width() / 2, Scene::Height() / 2 - 135.0), 900, 170, 10 }.draw(ColorF{ Palette::Black, 0.6});

	//進行ゲージ 黒い枠
	RoundRect{ Arg::center(Scene::Width() / 2, Scene::Height() / 2 - 50.0), 900, 10, 10 }.draw(ColorF{Palette::Black});
	//進行ゲージ 赤い部分
	Line{ starting_point, last_point }.draw(LineStyle::RoundCap, 5.0,  ColorF{ Palette::Red });

	PlayerHUD::GetInstance()->draw();

	//説明文描画
	FontAsset(U"TitleFont")(helpTexts[currentTextIndex]).drawAt(TextStyle::OutlineShadow(0.1, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }),28, Vec2(Scene::Width() / 2, Scene::Height() / 2 - 135.0));

	// START SKIP 点滅表示
	double blinkAlpha = Periodic::Square0_1(1.0); // カクカク点滅
	// double blinkAlpha = Periodic::Sine0_1(1.0); // フェード点滅にしたい場合はこちら
	ColorF blinkColor{ 1.0, 1.0, 1.0, blinkAlpha };
	FontAsset(U"TitleFont")(U"START SKIP").drawAt(TextStyle::OutlineShadow(0.1, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }),	23, Vec2(Scene::Width() / 2, Scene::Height() / 2 - 320.0), blinkColor);
}


void TutorialStage::NewInstance()
{
	if (instance != nullptr) delete instance;

	instance = new TutorialStage();
}

Vec2 TutorialStage::getGaugePosition(double startX, double endX, double duration, double startTime)
{
	double elapsed = Scene::Time() - startTime;
	double loopTime = Fmod(elapsed, duration);
	double rate = loopTime / duration;
	double x = Math::Lerp(startX, endX, rate);
	return Vec2{ x, 50.0 };
}

