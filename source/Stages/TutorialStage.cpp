# include "TutorialStage.hpp"
# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Objects/HitBox.hpp"
# include "../Characters/Player.hpp"
# include "../UI/PlayerHUD.hpp"
# include "../Characters/Enemies/Scarerun/Scarerun.hpp"

#define SIZE 20	//文字のサイズ
#define DRAW_POSITION Vec2{Scene::Width() / 2, Scene::Height() / 2 - 100.0} //座標

TutorialStage::TutorialStage()
{
	m_currentIndex = 0;

	// ステージの開始・終了ワールド座標を設定
	//プレイヤーの初期位を回質店
	m_stageStartX = 100.0;
	//Bossの生成位置を終了点
	m_stageEndX = (Scene::Width() / 2) + 4900.0;

	initialize();
}

void TutorialStage::initialize()
{
	// --- ロード画面表示 ---
	Scene::SetBackground(ColorF{ 0.0 }); // 背景を黒に設定
	FontAsset(U"TitleFont")(U"Loading...").drawAt(Scene::Center(), Palette::White);
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
		U"ようこそ『森の影』へ。\n敵を倒して強化し、森のボスを目指すアクションゲームです。",
		U"ボスを倒すことでステージクリアとなります。\nクリアタイムに応じてランクが表示されます。",
		U"左スティックを左右に倒すと、プレイヤーが移動します。\n実際に動いてみましょう。",
		U"Aボタンでジャンプできます。\n敵を飛び越えたり、攻撃を避けたりできます。",
		U"Xボタンで攻撃します。\n敵に接近して攻撃してみましょう。",
		U"Bボタンで回避攻撃ができます。\n移動しながら攻撃でき、敵をすり抜けることも可能です。\n使用後、再度使えるまで時間がかかります。",
		U"ジャンプ中にXボタンを押すと、空中攻撃ができます。\n空中の敵や高い位置にも攻撃が届きます。",
		U"敵を倒すと、バフアイテムが出現します。\n・赤いアイテム：攻撃力が上昇（DamageUp）\n・青いアイテム：移動速度が上昇（SpeedUp）\n近づくと自動で取得されます。",
		U"画面左上：現在のHP \n画面右上：ステージ開始時からの経過タイム \nステージクリア後、このタイムに応じてランクが表示されます。"
		

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
		if (HitBox* hitBox = dynamic_cast<HitBox*>(object))
		{
			hitBox->destroy();
		}
	}

	playerHUD->setPlayerHP(player->getHP());
	playerHUD->update();
	playerHUD->setPlayerState(player->getplayerstate());
	playerHUD->setPlayerVel(player->getBody().getVelocity());
	playerHUD->setPlayeravoid(player->getAvoidanceCooldown());
	playerHUD->setPlayerPosition(player->getBody().getPos());

	// 自動でテキストを切り替え
	if (autoSkipTimer.sF() > 7.0)
	{
		// 最後のテキストだった場合はゲームシーンへ遷移
		if (currentTextIndex == helpTexts.size() - 1)
		{
			sceneData().current_stage = 1;
			sceneChange(SceneState::Game, 0.5s);
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
	if (isLoading)
	{
		Scene::SetBackground(ColorF{ 0.0 });
		FontAsset(U"TitleFont")(U"Loading...").drawAt(Scene::Center(), Palette::White);
		return;
	}

	ClearPrint(); // 過去のPrint出力を消す
#ifdef _DEBUG

	Print << U"オブジェクト数: " << objects.size();

#endif

	{
		Stage::draw();
		PlayerHUD::GetInstance()->draw();
	}

	//説明文描画
	FontAsset(U"TitleFont")(helpTexts[currentTextIndex]).drawAt(TextStyle::OutlineShadow(0.1, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }),30, Vec2{ 640, 340 });

}

void TutorialStage::NewInstance()
{
	if (instance != nullptr) delete instance;

	instance = new TutorialStage();
}


