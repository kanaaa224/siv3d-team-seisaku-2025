# include "1.hpp"
# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Objects/HitBox.hpp"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/Scarerun/Scarerun.hpp"
# include "../Characters/Enemies/Flot/Flot.hpp"
# include "../UI/PlayerHUD.hpp"
# include "../Characters/Vaillant.hpp"

int Stage1::state = 0;

Stage1::Stage1()
{
	state = 0;

	initialize();
}

void Stage1::initialize()
{
	//BGM
	AudioAsset(U"Title_BGM").stop();
	AudioAsset(U"Battle_BGM").setVolume(0.7);
	AudioAsset(U"Battle_BGM").play();

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
}

void Stage1::update()
{
	Stage::update();

	static Stopwatch respawnTimer{ StartImmediately::No };

	float player_hp = 0;

	Player* player = nullptr;

	PlayerHUD* playerHUD = PlayerHUD::GetInstance();

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

		for (const auto& object : objects)
		{
			if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(object))
			{
				enemy->setPlayerPos(player->getBody().getPos());
			}

			if (Vaillant* vaillant = dynamic_cast<Vaillant*>(object))
			{
				vaillant->setPlayerPosition(player->getBody().getPos());
				playerHUD->setBossState(vaillant->getState());

				if (vaillant->getState() == VaillantState::Death)
				{
					static bool aaaaa = false;

					if (!aaaaa)
					{
						std::thread([this]()
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(4000));

							state = 1;

							aaaaa = false;
						}).detach();

						aaaaa = true;
					}
				}
			}

			if (StageBackground* stagebackground = dynamic_cast<StageBackground*>(object))
			{
				stagebackground->setCameraPos(camera.getTargetCenter() - Scene::Center());
			}

			if (HitBox* hitBox = dynamic_cast<HitBox*>(object))
			{
				hitBox->destroy();
			}

			if (BossEreaflg == false && player->getBody().getPos().x >= STAGE1_WIDTH - (Scene::Width() / 2))
			{
				if (Wall* wall = dynamic_cast<Wall*>(object))
				{
					wall->getBody().setPos(Vec2(camera.getCenter().x - Scene::Width() / 2, 500.0));
					BossEreaflg = true;
				}
			}
		}

		if (!excuted && BossEreaflg)
		{
			createObject<Wall>(Vec2(STAGE1_WIDTH, 500.0));
			excuted = true;
		}

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

	camera.update();
}

void Stage1::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

#ifdef _DEBUG
	Print << U"オブジェクト数: " << objects.size();
#endif

	{
		const auto t = camera.createTransformer();

		Stage::draw();
	}

	PlayerHUD::GetInstance()->draw();
}

void Stage1::NewInstance()
{
	if (instance != nullptr) delete instance;

	instance = new Stage1();
}
