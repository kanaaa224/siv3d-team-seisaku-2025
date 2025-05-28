# include "Vaillant.hpp"
# include "../Stages/Stage.hpp"
# include "Player.hpp"

Vaillant::Vaillant(P2World& world, const Vec2& position) : CharacterBase(world, position), start_position(position)
{
	body = world.createRect(P2Dynamic, position, SizeF{ 275, 300 });

	body.setFixedRotation(true);

	initialize();
}

void Vaillant::initialize()
{
	max_hp = 100;

	hp = max_hp;
}

void Vaillant::update()
{
	if (body.getPos().y >= 1000)
	{
		body.setPos(start_position);
		body.setVelocity(Vec2{});
	}

	if (!hp) Stage::GetInstance()->deleteObject(this);
}

void Vaillant::draw() const
{
	body.drawFrame();
}

void Vaillant::onHit(ObjectBase& object)
{
	if (Player* player = dynamic_cast<Player*>(&object))
	{
		if (object.getBody().getPos().y < (body.getPos().y - 100))
		{
			object.getBody().applyLinearImpulse(Vec2{ 0, -300 });

			onDamaged(10);
		}
		else
		{
			if (object.getBody().getPos().x < body.getPos().x)
			{
				object.getBody().applyLinearImpulse(Vec2{ -100, -100 });
			}
			else
			{
				object.getBody().applyLinearImpulse(Vec2{ 100, -100 });
			}

			player->onDamaged(10);
		}
	}
}

void Vaillant::onDamaged(float amount)
{
	CharacterBase::onDamaged(amount);
}

void Vaillant::onHealed(float amount)
{
	CharacterBase::onHealed(amount);
}
