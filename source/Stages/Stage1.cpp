# include "Stage1.hpp"

# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Objects/Player.hpp"
# include "../Objects/Enemy/Scarerun/Scarerun.hpp"
# include "../Objects/GameUI.hpp"
# include "../Objects/Hitbox.hpp"

# define DEBUG

Stage1::Stage1()
{
	this->initialize();
}

void Stage1::initialize()
{
	createObject<StageBackground>(Vec2{ 0, 0 });
	createObject<Ground>(Vec2{ 5000, (Scene::Height() + 5) });
	createObject<Wall>(Vec2{ -5, 500 });
	createObject<Scarerun>(Vec2{ 450, 500 });
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
		if (centerHeight < y) y = centerHeight;

		camera.setTargetCenter(Vec2{ x, y });

		for (const auto& object : objects)
		{
			if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(object))
			{
				enemy->setPlayerPos(player->getBody().getPos());
			}

			if (HitBox* hitbox = dynamic_cast<HitBox*>(object))
			{
				hitbox->setPlayerPos(player->getBody().getPos());
				hitbox->setPlayerState(player->getplayerstate());
				hitbox->setPlayerflip(player->getFlip());
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

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->setPlayerHP(player_hp);
	gameUI->update();
}

void Stage1::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

#ifdef DEBUG
	Print << U"オブジェクト数: " << objects.size();
#endif

	{
		const auto t = camera.createTransformer();

		Stage::draw();
	}

	GameUI::GetInstance()->draw();
}

void Stage1::NewInstance()
{
	if (instance != nullptr) delete instance;

	instance = new Stage1();
}
