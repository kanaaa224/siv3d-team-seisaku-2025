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

	AudioAsset(U"tutorial1_BGM").setVolume(0.3);
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

	initialize();
}

void TutorialStage::initialize()
{
	// --- ロード画面表示 ---
	Scene::SetBackground(ColorF{ 0.0 }); // 背景を黒に設定
	//Loding_image.resized(Size(1500, 500)).draw(-40, 30);
	//FontAsset(U"TitleFont")(U"Loading...").drawAt(Scene::Center(), Palette::White);
	
	Scene::SetBackground(ColorF{ 0.0 });

	Font font(30);
	Array<String> messages = {
		U"キノコからのドロップ品は攻撃力UP！",
		U"コウモリからのドロップ品はスピードUP！",
		U"コウモリにはジャンプ攻撃が有効だ！",
		U"バフを集めて強化！"
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
		font(randomMessage).draw(100, 150, Palette::Gray);

		//アニメーションさえるやつ
		const Vec2 basePos = Vec2{ Scene::Width() - 260, Scene::Height() - 60 };
		font(U"Now Loading").draw(basePos, Palette::White);
		font(dots).draw(basePos + Vec2{ 185, 0 }, Palette::White);

		//4秒以上経ったら抜ける
		if (elapsed > 4.0 && !shownOnce)
		{
			shownOnce = true;
			break;
		}
	}


	System::Update(); // 1フレーム強制描画

	//BGM
	/*AudioAsset(U"Title_BGM").stop();
	AudioAsset(U"Battle_BGM").setVolume(0.7);
	AudioAsset(U"Battle_BGM").play();*/

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
		U"ようこそ『森の影』へ！\nこのゲームは、敵を倒しながら成長し、\n森の奥にいるボスに挑むアクションゲームです。",
        U"ボスを倒すとステージクリア！\nクリアまでにかかった時間に応じてランクが決まります。",
        U"左スティックを左右に倒すと、キャラが移動します。\nまずは動いてみましょう！",
        U"Aボタンでジャンプできます。\n敵を飛び越えたり、攻撃を避けたりするのに使います。",
        U"Xボタンで攻撃ができます。\n敵に近づいて、実際に攻撃してみましょう！",
        U"Bボタンを押すと回避攻撃！\n素早く移動しながら攻撃できます。\n使ったあとは少しの間使えなくなります。",
        U"ジャンプ中にXボタンを押すと、空中攻撃ができます。\n空を飛んでいる敵にも当てられます。",
        U"ジャンプ中にBボタンを押すと、空中回避ができます。\n敵の攻撃をすり抜けながら反撃するチャンス！\nこちらも使ったあと少し待つ必要があります。",
        U"敵を倒すとバフアイテムが出てくることがあります。\n・赤：攻撃力アップ（DamageUp）\n・青：移動速度アップ（SpeedUp）\n近づくだけで自動で拾えます。",
        U"画面左上に注目！\nオレンジのゲージがHP、隣の数字が経過時間です。\nタイムはステージクリア後のランクに関係しています。"

	};

	for (const auto& section : helpTexts)
	{
		FontAsset(U"TitleFont")(section).draw(-9999, -9999); // ウォームアップ
	}

	isLoading = false; // 読み込み完了
	autoSkipTimer.start();
}

void TutorialStage::update()
{


	auto controller = XInput(0);

	Stage::update();

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

	// ゲージ更新
	if (gaugeStarted && sceneData().current_stage != 1)
	{
		last_point.x = getGaugePosition(195, 1085, HELP_TIMER, gaugeStartTime).x;
	}

	// 自動でテキストを切り替え
	if (last_point.x >= 1084)
	{
		// 最後のテキストだった場合はゲームシーンへ遷移
		if (currentTextIndex == helpTexts.size() - 1.0)
		{
			sceneData().current_stage = 1;
			last_point.x = 1085.0;
			sceneChange(SceneState::Game, 1.0s);
		}
		else
		{
			currentTextIndex = (currentTextIndex + 1);
			autoSkipTimer.restart();
		}
	}
	//スキップ
	if (KeyEnter.down() || Gamepad(0).isConnected() && Gamepad(0).buttons[7].down())
	{
		sceneData().current_stage = 1;

		sceneChange(SceneState::Game, 0.5s);
	}

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

	//四角形描画
	RoundRect{ Arg::center(Scene::Width() / 2, Scene::Height() / 2 - 135.0), 900, 170, 10 }.draw(ColorF{ Palette::Black, 0.6});

	//進行ゲージ 黒い枠
	RoundRect{ Arg::center(Scene::Width() / 2, Scene::Height() / 2 - 50.0), 900, 10, 10 }.draw(ColorF{Palette::Black});
	//進行ゲージ 赤い部分
	Line{ starting_point, last_point }.draw(LineStyle::RoundCap, 5.0,  ColorF{ Palette::Red });

	PlayerHUD::GetInstance()->draw();

	//説明文描画
	FontAsset(U"TitleFont")(helpTexts[currentTextIndex]).drawAt(TextStyle::OutlineShadow(0.1, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }),28, Vec2(Scene::Width() / 2, Scene::Height() / 2 - 135.0));

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

