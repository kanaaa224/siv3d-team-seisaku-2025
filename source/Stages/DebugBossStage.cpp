# include "DebugBossStage.hpp"
# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Objects/HitBox.hpp"
# include "../Objects/Leaf.h"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/Scarerun/Scarerun.hpp"
# include "../Characters/Enemies/Flot/Flot.hpp"
# include "../Characters/Vaillant.hpp"
# include "../Characters/Slime.hpp"
# include "../UI/PlayerHUD.hpp"
# include "../UI/EnemyBoss.hpp"
# include "../Utils/TimerUtils.hpp"

using namespace TimerUtils;
using namespace std::chrono_literals;

DebugBossStage::DebugBossStage()
{
	initialize();
}

void DebugBossStage::initialize()
{
	createObject<StageBackground>();

	createObject<Ground>(Vec2{ 5000, (Scene::Height() + 5) });
	createObject<Wall>  (Vec2{ -5, 500 });
	createObject<Wall>  (Vec2{ Scene::Width() + 5, 500 });

	createObject<Vaillant>(Scene::Center() + Vec2{ 100, 0 });
	createObject<Player>  (Scene::Center() - Vec2{ 100, 0 });

	camera = Camera2D(Scene::Center(), 1.0, CameraControl::None_);
}

void DebugBossStage::update()
{
	Stage::update();

	Player* player = nullptr;

	for (const auto& object : objects) if ((player = dynamic_cast<Player*>(object))) break;

	if (player)
	{
		double x = player->getBody().getPos().x;
		double y = player->getBody().getPos().y;

		/*if (centerWidth  > x)*/ x = Scene::Center().x;
		/*if (centerHeight < y)*/ y = Scene::Center().y;

		camera.setTargetCenter(Vec2{ x, y });

		auto _objects_ = objects;

		for (const auto& object : _objects_)
		{
			if (Vaillant* vaillant = dynamic_cast<Vaillant*>(object))
			{
				vaillant->setPlayerPosition(player->getplayerstate() != ePlayerState::die ? player->getBody().getPos() : Vec2{ 0, 0 });
				vaillant->setForbidJump(player->getplayerstate() <= ePlayerState::jump);

				if (player->getHP() == 0.0f) vaillant->setAttackStarted(false);

				EnemyBossUI* enemyBossUI = EnemyBossUI::GetInstance();

				enemyBossUI->setHP(vaillant->getHP());
				enemyBossUI->setName(U"バイラント");
				enemyBossUI->update();

				if (vaillant->getState() == VaillantState::Attack &&
					vaillant->getAttackType() == VaillantAttackType::Rush &&
					vaillant->getAttackState() == VaillantAttackState::Attacked)
				{
					createObject<Leaf>(Vec2{ Scene::Width() - Random(0, Scene::Width()), Random(-(Scene::Height() / 1.5), 0.0)}, Random(0, 4), eDropPostion::eCenter, Random(10, 30));
				}
			}

			if (Slime* slime = dynamic_cast<Slime*>(object))
			{
				slime->setPlayerPosition(player->getBody().getPos());
			}

			if (StageBackground* stageBackground = dynamic_cast<StageBackground*>(object))
			{
				stageBackground->setCameraPos(camera.getTargetCenter() - Scene::Center());
			}

			if (HitBox* hitBox = dynamic_cast<HitBox*>(object))
			{
				hitBox->destroy();
			}
		}
	}

	camera.update();

#ifdef _DEBUG
	if (Key0.down()) sceneChange(SceneState::Title, 0.5s);
	if (Key1.down()) createObject<Vaillant>(Scene::Center());
	if (Key2.down()) createObject<Player>  (Scene::Center());
#endif
}

void DebugBossStage::draw() const
{
	ClearPrint();

#ifdef _DEBUG
	Print << U"オブジェクト数: " << objects.size();
#endif

	double shouldShake = 0.0;

	Vec2 shakeOffset = Vec2{ 0, 0 };

	for (const auto& object : objects)
	{
		if (Vaillant* vaillant = dynamic_cast<Vaillant*>(object))
		{
			if (vaillant->getState() == VaillantState::Attack &&
				vaillant->getAttackType() == VaillantAttackType::Earthquake &&
				vaillant->getAttackState() == VaillantAttackState::Attacking)
			{
				shouldShake = 5.0;
				break;
			}

			if (vaillant->getState() == VaillantState::Attack &&
				vaillant->getAttackType() == VaillantAttackType::Rush &&
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
}

void DebugBossStage::NewInstance()
{
	DeleteInstance();

	instance = new DebugBossStage();
}
