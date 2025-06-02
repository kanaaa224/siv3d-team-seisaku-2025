# include "1.hpp"
# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/Scarerun/Scarerun.hpp"
# include "../Characters/Enemies/Flot/Flot.hpp"
# include "../UI/PlayerHUD.hpp"
# include "../Objects/Hitbox.hpp"
# include "../Characters/Vaillant.hpp"

Stage1::Stage1()
{
	initialize();
}

void Stage1::initialize()
{
	//ステージオブジェクト
	createObject<StageBackground>(Vec2{ -640, 0 });
	createObject<Ground>         (Vec2{ 5000, (Scene::Height() + 5) });
	createObject<Wall>           (Vec2{ -5, 500 });

	//敵
	createObject<Scarerun>(Vec2{ 450, 500 });
	createObject<Flot>    (Vec2{ 1000,500 });

	//ボス
	createObject<Vaillant>(Vec2{ (Scene::Width() / 2) + 1000, 500 });

	//プレイヤー
	createObject<Player>(Vec2{ (Scene::Width() / 2), 650 });

	camera = Camera2D(Vec2{ (Scene::Width() / 2), (Scene::Height() / 2) }, 1.0, CameraControl::None_);
}

void Stage1::update()
{
	Stage::update();

	static Stopwatch respawnTimer{ StartImmediately::No };

	float player_hp = 0;

	Player* player = nullptr;

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

		int centerWidth  = (Scene::Width()  / 2);
		int centerHeight = (Scene::Height() / 2);

		if (centerWidth  > x) x = centerWidth;
		/*if (centerHeight < y)*/ y = centerHeight;

		camera.setTargetCenter(Vec2{ x, y });

		for (const auto& object : objects)
		{
			if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(object))
			{
				enemy->setPlayerPos(player->getBody().getPos());
			}
			
			if (Vaillant* vaillant = dynamic_cast<Vaillant*>(object))
			{
				vaillant->setPlayerPosition(player->getBody().getPos());
			}

			if (HitBox* hitbox = dynamic_cast<HitBox*>(object))
			{
				hitbox->setPlayerPos(player->getBody().getPos());
				hitbox->setPlayerState(player->getplayerstate());
				hitbox->setPlayerflip(player->getFlip());
			}

			if (StageBackground* stagebackground = dynamic_cast<StageBackground*>(object))
			{
				stagebackground->setCameraPos(camera.getCenter());
			}
		}

		PlayerHUD* playerHUD = PlayerHUD::GetInstance();

		playerHUD->setPlayerHP(player_hp);
		playerHUD->update();
		playerHUD->setPlayerState(player->getplayerstate());
		playerHUD->setPlayerVel(player->getBody().getVelocity());
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
