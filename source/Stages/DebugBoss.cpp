# include "DebugBoss.hpp"
# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Characters/Vaillant.hpp"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/EnemyBase.hpp"
# include "../UI/PlayerHUD.hpp"

StageDebugBoss::StageDebugBoss()
{
	initialize();
}

void StageDebugBoss::initialize()
{
	createObject<StageBackground>(Vec2{ 0, 0 });
	createObject<Ground>(Vec2{ 5000, (Scene::Height() + 5) });
	createObject<Wall>(Vec2{ -5, 500 });
	createObject<Vaillant>(Vec2{ (Scene::Width() / 2) + 200, 500 });
	createObject<Player>(Vec2{ (Scene::Width() / 2), 500 });

	camera = Camera2D(Vec2{ (Scene::Width() / 2), (Scene::Height() / 2) }, 1.0, CameraControl::None_);
}

void StageDebugBoss::update()
{
	Stage::update();

	static Stopwatch respawnTimer{ StartImmediately::No };

	Player* player = nullptr;

	for (const auto& object : objects)
	{
		if ((player = dynamic_cast<Player*>(object))) break;
	}

	if (player)
	{
		respawnTimer.reset();

		double x = player->getBody().getPos().x;
		double y = player->getBody().getPos().y;

		int centerWidth  = (Scene::Width()  / 2);
		int centerHeight = (Scene::Height() / 2);

		if (centerWidth  > x) x = centerWidth;
		if (centerHeight < y) y = centerHeight;

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
		}
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

	PlayerHUD* playerHUD = PlayerHUD::GetInstance();

	playerHUD->update();
}

void StageDebugBoss::draw() const
{
	ClearPrint();

#ifdef _DEBUG
	Print << U"オブジェクト数: " << objects.size();
#endif

	{
		const auto t = camera.createTransformer();

		Stage::draw();
	}

	PlayerHUD::GetInstance()->draw();
}

void StageDebugBoss::NewInstance()
{
	if (instance != nullptr) delete instance;

	instance = new StageDebugBoss();
}
