# include "HitBox.hpp"
# include "../Characters/Enemies/EnemyBase.hpp"
# include "../Characters/Player.hpp"

bool HitBox::flg = false;

HitBox::HitBox(P2World& world, const Vec2& position) : ObjectBase(world, position)
{
	body = world.createRectSensor(
		P2Kinematic,
		position,
		RectF{ 20, 90 },
		// 物体の衝突判定の設定 ただし、すり抜けはできない
		P2Filter{

			// 自分のカテゴリ設定
			.categoryBits = CollisionCategory::Box1,
			// Hitさせたいカテゴリを選ぶ
			.maskBits = CollisionCategory::Enemy
		}
	);

	initialize();
}

void HitBox::initialize()
{
	if (flg == true) deleteSelf();

	flg = true;
}

void HitBox::update()
{
	if (flip_flg == false)
	{
		body.setPos(Vec2(playerPos.x + 28, playerPos.y - 45));
	}
	else if(flip_flg == true)
	{
		body.setPos(Vec2(playerPos.x - 48, playerPos.y - 45));
	}

	if (playerState != ePlayerState::attack)
	{
		deleteSelf();
		flg = false;
	}
}

void HitBox::draw() const
{
	body.drawFrame(1.0, ColorF(Palette::Red));
}

void HitBox::onHit(ObjectBase& object)
{
	if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(&object))
	{
		enemy->onDamaged(50);
		deleteSelf();
		flg = false;
	}
}
