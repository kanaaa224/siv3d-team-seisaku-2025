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

#endif

	{
		Stage::draw();
		PlayerHUD::GetInstance()->draw();
	}
}

void TutorialStage::NewInstance()
{
	if (instance != nullptr) delete instance;

	instance = new TutorialStage();
}


