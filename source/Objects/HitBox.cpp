# include "HitBox.hpp"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/EnemyBase.hpp"
# include "../Characters/Vaillant.hpp"

HitBox::HitBox(P2World& world, const Vec2& position, ObjectBase& owner, float damageAmount) : ObjectBase(world, position), owner(&owner), damageAmount(damageAmount)
{
	body = world.createRectSensor(
		P2Kinematic,
		position,
		RectF { 20, 90 },
		P2Filter {
			.categoryBits = CollisionCategory::HitBox, // 自分のカテゴリ
			.maskBits     = CollisionCategory::Enemy   // Hitさせたいカテゴリ
		}
	);
}

void HitBox::update()
{
	if (Player* player = dynamic_cast<Player*>(owner))
	{
		Vec2 pos = player->getBody().getPos();

		if (player->getFlip())
		{
			body.setPos({ pos.x - 48, pos.y - 45 });
		}
		else
		{
			body.setPos({ pos.x + 28, pos.y - 45 });
		}

		if (player->getplayerstate() != ePlayerState::attack) destroy();
	}
}

void HitBox::draw() const
{
#ifdef _DEBUG
	body.drawFrame(1.0, Palette::Red);
#endif
}

void HitBox::onHit(ObjectBase& object)
{
	if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(&object))
	{
		enemy->applyDamage(damageAmount);

		destroy();
	}

	if (Vaillant* vaillant = dynamic_cast<Vaillant*>(&object))
	{
		vaillant->applyDamage(damageAmount);

		destroy();
	}
}
