# include "Stage1.hpp"
# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Objects/HitBox.hpp"
# include "../Objects/Leaf.h"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/Scarerun/Scarerun.hpp"
# include "../Characters/Enemies/Flot/Flot.hpp"
# include "../UI/PlayerHUD.hpp"
# include "../UI/EnemyBoss.hpp"
# include "../Characters/Vaillant.hpp"
# include "../Characters/Slime.hpp"
# include "../Utils/TimerUtils.hpp"

using namespace TimerUtils;
using namespace std::chrono_literals;

Stage1::Stage1()
{
	initialize();
}

void Stage1::initialize()
{
	//BGM
	AudioAsset(U"Title_BGM").stop();
	AudioAsset(U"tutorial1_BGM").stop();
	AudioAsset(U"Battle_BGM").setVolume(0.7);
	AudioAsset(U"Battle_BGM").setLoop(true);
	AudioAsset(U"Battle_BGM").play();


	// ステージの開始・終了ワールド座標を設定
	//プレイヤーの初期位を回質店
	m_stageStartX = 100.0;
	//Bossの生成位置を終了点
	m_stageEndX = (Scene::Width() / 2) + 4900.0;

	//ステージオブジェクト
	createObject<StageBackground>();
	createObject<Ground>(Vec2{ STAGE1_WIDTH, (Scene::Height() + 5) });
	createObject<Wall>(Vec2{ -5, 500 });

	for (int8 i = 1; i < 6; i++)
	{
		//敵
		createObject<Scarerun>(Vec2{ 700 * i, Scene::Height() - 45 });
		createObject<Flot>(Vec2{ 800 * i ,500 });
	}

	//ボス
	//createObject<Vaillant>(Vec2{ (Scene::Width() / 2) + 4300, 500 });

	//プレイヤー
	createObject<Player>(Vec2{ (100), 650 });

	camera = Camera2D(Vec2{ (Scene::Width() / 2), (Scene::Height() / 2) }, 1.0, CameraControl::None_);

	BossEreaflg = false;
	excuted = false;
	boss_spawn = false;

	PlayerHUD::GetInstance()->resetTime();
}

void Stage1::update()
{
	Stage::update();

	static Stopwatch respawnTimer{ StartImmediately::No };

	float player_hp = 0;

	Player* player = nullptr;

	PlayerHUD* playerHUD = PlayerHUD::GetInstance();

	playerHUD->player_game_world_start_x = m_stageStartX;
	playerHUD->player_game_world_end_x = m_stageEndX;

	for (const auto& object : objects)
	{
		if ((player = dynamic_cast<Player*>(object))) break;
	}

	if (player)
	{
		respawnTimer.reset();

		player_hp = player->getHP();

		double x = player->getBody().getPos().x;
		double y = player->getBody().getPos().y;

		if (BossEreaflg == false)
		{
			int centerWidth = (Scene::Width() / 2);
			int centerHeight = (Scene::Height() / 2);

			if (centerWidth > x) x = centerWidth;
			/*if (centerHeight < y)*/ y = centerHeight;

			camera.setTargetCenter(Vec2{ x, y });
		}

		auto _objects_ = objects;

		for (const auto& object : _objects_)
		{
			if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(object))
			{
				enemy->setPlayerPos(player->getBody().getPos());
			}

			if (Vaillant* vaillant = dynamic_cast<Vaillant*>(object))
			{
				vaillant->setPlayerPosition(player->getplayerstate() != ePlayerState::die ? player->getBody().getPos() : Vec2{ 0, 0 });
				vaillant->setForbidJump(player->getplayerstate() <= ePlayerState::jump);

				if (player_hp == 0.0f) vaillant->setAttackStarted(false);

				EnemyBossUI* enemyBossUI = EnemyBossUI::GetInstance();

				enemyBossUI->setHP(vaillant->getHP());
				enemyBossUI->setName(U"バイラント");
				enemyBossUI->update();

				playerHUD->setBossState(vaillant->getState());

				// GameClear 遷移条件
				if (vaillant->getState() == VaillantState::Death)
				{
					static bool aaaaa = false;

					sceneData().clearTime = playerHUD->getClearTime();

					if (!aaaaa)
					{
						SetTimeout([this] { aaaaa = false; sceneChange(SceneState::Clear, 0.5s); }, 5000ms);

						aaaaa = true;
					}
				}

				if (vaillant->getState()       == VaillantState::Attack &&
					vaillant->getAttackType()  == VaillantAttackType::Rush &&
					vaillant->getAttackState() == VaillantAttackState::Attacked)
				{
					createObject<Leaf>(Vec2{ STAGE1_WIDTH - Random(0, Scene::Width()), Random(-(Scene::Height() / 1.5), 0.0) }, Random(0, 4), eDropPostion::eCenter, Random(10, 30));
				}
			}

			if (Slime* slime = dynamic_cast<Slime*>(object))
			{
				slime->setPlayerPosition(player->getBody().getPos());
			}

			if (StageBackground* stagebackground = dynamic_cast<StageBackground*>(object))
			{
				stagebackground->setCameraPos(camera.getTargetCenter() - Scene::Center());
			}

			if (HitBox* hitBox = dynamic_cast<HitBox*>(object))
			{
				hitBox->destroy();
			}

			//敵を削除
			if (BossEreaflg)
			{
				if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(object))
				{
					enemy->destroy();
				}
			}

			//左壁移動
			if (BossEreaflg == false && player->getBody().getPos().x >= STAGE1_WIDTH - (Scene::Width() / 2))
			{
				if (Wall* wall = dynamic_cast<Wall*>(object))
				{
					wall->getBody().setPos(Vec2(STAGE1_WIDTH - Scene::Width(), 500.0));
					BossEreaflg = true;
				}
			}
		}

		// GameOver 遷移条件
		if (player->getplayerstate() == ePlayerState::die)
		{
			static bool aaaaa = false;

			if (!aaaaa)
			{
				SetTimeout([this] { aaaaa = false; sceneChange(SceneState::GameOver, 0.5s); }, 3000ms);

				aaaaa = true;
			}
		}

		// 右壁
		if (!excuted && BossEreaflg)
		{
			createObject<Wall>(Vec2(STAGE1_WIDTH, 500.0));
			excuted = true;
		}

		// ボス生成
		if (!boss_spawn && BossEreaflg)
		{
			createObject<Vaillant>(Vec2{ (Scene::Width() / 2) + 4900, 500 });
			boss_spawn = true;
		}

		playerHUD->setPlayerHP(player->getHP());
		playerHUD->update();
		playerHUD->setPlayerState(player->getplayerstate());
		playerHUD->setPlayerVel(player->getBody().getVelocity());
		playerHUD->setPlayeravoid(player->getAvoidanceCooldown());
		playerHUD->setBossEreaFlg(BossEreaflg);
		playerHUD->setPlayerPosition(player->getBody().getPos());

		//バフの数を取得、HUDに設定
		playerHUD->setBuffDamageUpCount(player->GetPlayerBuffDamageUpCount());
		playerHUD->setBuffSpeedUpCount(player->GetPlayerBuffSpeedUpCount());
	}
#ifdef _DEBUG
	else
	{
		if (!respawnTimer.isRunning()) respawnTimer.restart();

		if (respawnTimer.sF() >= 1.0)
		{
			createObject<Player>(Vec2{ (Scene::Width() / 2), 500 });

			respawnTimer.reset();
		}
	}
	
	if (Key0.down()) sceneChange(SceneState::Title, 0.5s);
#endif

	camera.update();
}

void Stage1::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

#ifdef _DEBUG
	Print << U"オブジェクト数: " << objects.size();
#endif

	double shouldShake = 0.0;

	Vec2 shakeOffset = Vec2{ 0, 0 };

	for (const auto& object : objects)
	{
		if (Vaillant* vaillant = dynamic_cast<Vaillant*>(object))
		{
			if (vaillant->getState()       == VaillantState::Attack &&
				vaillant->getAttackType()  == VaillantAttackType::Earthquake &&
				vaillant->getAttackState() == VaillantAttackState::Attacking)
			{
				shouldShake = 5.0;
				break;
			}

			if (vaillant->getState()       == VaillantState::Attack &&
				vaillant->getAttackType()  == VaillantAttackType::Rush &&
				vaillant->getAttackState() == VaillantAttackState::Attacked)
			{
				shouldShake = 10.0;
				break;
			}
		}
	}

	if (shouldShake) shakeOffset = RandomVec2(Circle{ shouldShake });

	{
		const auto t = camera.createTransformer();

		const Transformer2D shakeTransform(Mat3x2::Translate(shakeOffset), TransformCursor::Yes);

		Stage::draw();
	}

	for (const auto& object : objects)
	{
		if (Vaillant* vaillant = dynamic_cast<Vaillant*>(object)) EnemyBossUI::GetInstance()->draw();
	}

	PlayerHUD::GetInstance()->draw();
}

void Stage1::NewInstance()
{
	DeleteInstance();

	instance = new Stage1();
}
