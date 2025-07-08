# include "Tentacles.hpp"
# include "../Utils/TimerUtils.hpp"

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

void Tentacles::update()
{
	frameTime += Scene::DeltaTime();

	if (body) position = body.getPos();

	if (position.y >= (Scene::Height() + 100))
	{
		body.setPos(start_position);
		body.setVelocity({ 0, 0 });
	}
}

void Tentacles::draw() const
{
	body.drawFrame();
	
	String assetName = U"Vaillant Tentacles 1";
	
	Vec2 margin{ 0, 0 };

	SizeF cutoutSize{ 48, 48 };

	SizeF resized = cutoutSize * 5;
	
	Vec2 shiftAmount{ 0, -70 };
	
	ColorF mask{ 1.0, 1.0, 1.0, 1.0 };
	
	double frameDuration = 0.085;
	int    frameCount    = 16;
	
	int currentFrame = static_cast<int>(frameTime / frameDuration) % frameCount;

	if (currentFrame) margin.x += (cutoutSize.x) * currentFrame;
	
	if (assetName) TextureAsset(assetName)(margin, cutoutSize).resized(resized).drawAt(position + shiftAmount, mask);
}

void Tentacles::onHit(ObjectBase& object, const P2Collision& collision) {}

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
