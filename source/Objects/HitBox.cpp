# include "HitBox.hpp"
# include "../Characters/Player.hpp"
# include "../Characters/Enemies/EnemyBase.hpp"
# include "../Characters/Vaillant.hpp"
# include "../Characters/Slime.hpp"
# include "../Characters/Tentacles.hpp"

HitBox::HitBox(P2World& world, const Vec2& position, ObjectBase& owner) : ObjectBase(world, position), owner(&owner)
{
	SizeF size;

	HitBox::owner = &owner;

	// プレイヤーが出したヒットボックスだったとき
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

	// ボス1が出したヒットボックスだったとき
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

void HitBox::update() {}

void HitBox::draw() const
{
#ifdef _DEBUG
	body.drawFrame(1.0, Palette::Red);
#endif
}

void HitBox::onHit(ObjectBase& object)
{
	// プレイヤーに当たったとき
	if (Player* player = dynamic_cast<Player*>(&object))
	{
		// ボス1が出したヒットボックスだったとき
		if (Vaillant* vaillant = dynamic_cast<Vaillant*>(owner))
		{
			if (vaillant->getState() < VaillantState::Death &&

				player->getplayerstate() != ePlayerState::avoidance &&
				player->getplayerstate() != ePlayerState::jump_attack &&
				player->getplayerstate() != ePlayerState::jump_avoidance)
			{
				player->applyDamage(20.0f);
			}
		}

		else
		{
			player->applyDamage(10.0f);
		}

		destroy();
	}

	// 敵に当たったとき
	if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(&object))
	{
		// プレイヤーが出したヒットボックスだったとき
		if (Player *player = dynamic_cast<Player*>(owner)) {
			enemy->applyDamage(player->GetPlayerAttackDamage());

			player->SetHitEnemy(true);
		}

		destroy();
	}

	// ボス1に当たったとき
	if (Vaillant* vaillant = dynamic_cast<Vaillant*>(&object))
	{
		// プレイヤーが出したヒットボックスだったとき
		if (Player* player = dynamic_cast<Player*>(owner))
		{
			if (!(vaillant->getState()       == VaillantState::Attack &&
				  vaillant->getAttackType()  == VaillantAttackType::Teleport) &&
				  vaillant->getState()        < VaillantState::Death)
			{
				vaillant->applyDamage(player->GetPlayerAttackDamage());

				player->SetHitEnemy(true);
			}
		}

		destroy();
	}

	// スライムに当たったとき
	if (Slime* slime = dynamic_cast<Slime*>(&object))
	{
		// プレイヤーが出したヒットボックスだったとき
		if (Player* player = dynamic_cast<Player*>(owner))
		{
			slime->applyDamage(slime->getMaxHP() / 2);

			player->SetHitEnemy(true);
		}

		destroy();
	}
	
	// 触手に当たったとき
	if (Tentacles* tentacles = dynamic_cast<Tentacles*>(&object))
	{
		// プレイヤーが出したヒットボックスだったとき
		if (Player* player = dynamic_cast<Player*>(owner))
		{
			tentacles->applyDamage(25.0f);

			player->SetHitEnemy(true);
		}

		destroy();
	}
}
