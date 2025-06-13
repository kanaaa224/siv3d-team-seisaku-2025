# include "HitBox.hpp"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/EnemyBase.hpp"
# include "../Characters/Vaillant.hpp"

HitBox::HitBox(P2World& world, const Vec2& position, ObjectBase& owner) : ObjectBase(world, position), owner(&owner)
{
	SizeF size;

	if (Player* player = dynamic_cast<Player*>(this->owner))
	{
		if (player->getplayerstate() == ePlayerState::avoidance ||
			player->getplayerstate() == ePlayerState::jump_avoidance)
		{
			size = { 150, 90 };
		}
		else
		{
			size = { 95, 90 };
		}

		body = world.createRectSensor(
		P2Kinematic,
		position,
		RectF { size },
		P2Filter {
			.categoryBits = CollisionCategory::HitBox, // 自分のカテゴリ
			.maskBits     = CollisionCategory::Enemy   // Hitさせたいカテゴリ
		}
	);
	}

	if (Vaillant* vaillant = dynamic_cast<Vaillant*>(this->owner))
	{
		size = { 400, 100 };

		body = world.createRectSensor(
			P2Kinematic,
			position,
			RectF { size },
			P2Filter {
				.categoryBits = CollisionCategory::HitBox,
				.maskBits     = CollisionCategory::Player
			}
		);
	}
}

void HitBox::update()
{

}

void HitBox::draw() const
{
#ifdef _DEBUG
	body.drawFrame(1.0, Palette::Red);
#endif
}

void HitBox::onHit(ObjectBase& object)
{
	if (Player* player = dynamic_cast<Player*>(&object))
	{
		player->applyDamage(30);

		destroy();
	}

	if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(&object))
	{
		enemy->applyDamage(20);

		destroy();
	}

	if (Vaillant* vaillant = dynamic_cast<Vaillant*>(&object))
	{
		vaillant->applyDamage(10);

		destroy();
	}
}
