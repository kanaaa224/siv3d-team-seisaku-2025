# include "HitBox.hpp"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/EnemyBase.hpp"
# include "../Characters/Vaillant.hpp"
# include "../Characters/Slime.hpp"

HitBox::HitBox(P2World& world, const Vec2& position, ObjectBase& owner) : ObjectBase(world, position), owner(&owner)
{
	SizeF size;

	HitBox::owner = &owner;

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
		player->applyDamage(20.f);

		destroy();
	}

	if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(&object))
	{
		if (Player *player = dynamic_cast<Player*>(owner)) {
			enemy->applyDamage(player->GetPlayerAttackDamage());
			destroy();
		}
	}

	if (Vaillant* vaillant = dynamic_cast<Vaillant*>(&object))
	{
		if (Player* player = dynamic_cast<Player*>(owner)) {
			vaillant->applyDamage(player->GetPlayerAttackDamage());

			//vaillant->getBody().applyLinearImpulse(vaillant->getBody().getPos().x < body.getPos().x ? Vec2{ -10, 0 } : Vec2{ 10, 0 });

			destroy();
		}
	}

	if (Slime* slime = dynamic_cast<Slime*>(&object))
	{
		if (Player* player = dynamic_cast<Player*>(owner)) {
			slime->applyDamage(slime->getMaxHP() / 2);

			slime->getBody().applyLinearImpulse(slime->getBody().getPos().x < body.getPos().x ? Vec2{ -10, 0 } : Vec2{ 10, 0 });

			destroy();
		}
	}
}
