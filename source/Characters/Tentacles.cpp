# include "Tentacles.hpp"
# include "../Utils/Timer.hpp"

using namespace TimerUtils;
using namespace std::chrono_literals;

Tentacles::Tentacles(P2World& world, const Vec2& position) :
	CharacterBase(world, position),

	start_position(position)
{
	body = world.createRect(P2Dynamic, position, size = { TENTACLES_SIZE }, { .density = 0.0, .friction = 0.75 }, { .categoryBits = CollisionCategory::Enemy, .maskBits = CollisionCategory::All & ~CollisionCategory::Player & ~CollisionCategory::Enemy });

	body.addRectSensor(RectF{ -(size / 2), size }, { .categoryBits = CollisionCategory::Enemy, .maskBits = CollisionCategory::Player });

	body.setFixedRotation(true);
	
	hp = max_hp = TENTACLES_MAX_HP;
}

void Tentacles::update() {}

void Tentacles::draw() const
{
	body.drawFrame();
}

void Tentacles::onHit(ObjectBase& object) {}

void Tentacles::onDamaged(float amount)
{
	if (!damaged)
	{
		CharacterBase::onDamaged(amount);

		SetTimeout([this] { damaged = false; }, 750ms);

		damaged = true;
	}
}

void Tentacles::destroy()
{
	if (!destroy_executed)
	{
		body.release();

		SetTimeout([this] { ObjectBase::destroy(); }, 2000ms);

		destroy_executed = true;
	}
}

void Tentacles::die()
{
	if (!die_executed)
	{
		body.setVelocity({ 0, 0 });

		SetTimeout([this] { CharacterBase::die(); }, 2000ms);

		die_executed = true;
	}
}
