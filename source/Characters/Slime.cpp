/* Copied from "https://github.com/kanaaa224/siv3d-2d-scroll" */

# include "Slime.hpp"
# include "../Utils/Timer.hpp"

using namespace TimerUtils;
using namespace std::chrono_literals;

namespace
{
	Array<Vec2> ChaikinSmooth(const Array<Vec2>& inputVertices, int iterations = 2)
	{
		if (inputVertices.size() < 3) throw std::invalid_argument("Input vertices must form a polygon (at least 3 vertices).");
		if (iterations <= 0)          throw std::invalid_argument("Iterations must be a positive integer.");

		Array<Vec2> currentVertices = inputVertices;

		for (int k = 0; k < iterations; ++k)
		{
			Array<Vec2> newVertices;

			const size_t vertexCount = currentVertices.size();

			for (size_t i = 0; i < vertexCount; ++i)
			{
				const Vec2& p0 = currentVertices[i];
				const Vec2& p1 = currentVertices[(i + 1) % vertexCount];

				const Vec2 q = p0 * 0.75 + p1 * 0.25;
				const Vec2 r = p0 * 0.25 + p1 * 0.75;

				newVertices << q << r;
			}

			currentVertices = newVertices;
		}

		return currentVertices;
	}
}

Slime::Slime(P2World& world, const Vec2& position) : CharacterBase(world, position), centerRadius(20.0), outerRadius(10.0), mirrored(false), damaged(false)
{
	P2Material material{ .friction = 0.5 };

	P2Filter filter{ .categoryBits = CollisionCategory::Enemy };

	body = world.createCircle(P2Dynamic, position, Circle(centerRadius), material, filter);

	const int32 num = 12;

	const double diffAngle = Math::TwoPi / num;

	for (int32 i = 0; i < num; ++i)
	{
		const Vec2 offset = Vec2::Right().rotated(diffAngle * i) * (centerRadius + outerRadius);

		aroundBodies << world.createCircle(P2Dynamic, position + offset, Circle(outerRadius), material, filter);
	}

	for (int32 i = 0; i < num; ++i)
	{
		const Vec2 jointPosA = body.getPos();
		const Vec2 jointPosB = aroundBodies[i].getPos();

		joints << world.createDistanceJoint(body, jointPosA, aroundBodies[i], jointPosB, (jointPosB - jointPosA).length()).setLinearStiffness(3.0, 0.5).setMinLength(0.0);
	}

	for (int32 i = 0; i < num; ++i)
	{
		const Vec2& posA = aroundBodies[i].getPos();
		const Vec2& posB = aroundBodies[(i + 1) % num].getPos();

		joints << world.createDistanceJoint(aroundBodies[i], posA, aroundBodies[(i + 1) % num], posB, (posB - posA).length()).setLinearStiffness(1.0, 0.5).setMinLength(0.0);
	}

	hp = max_hp = SLIME_MAX_HP;

	body.applyLinearImpulse({ 0, -SLIME_JUMP_POWER });
}

void Slime::update()
{
	if (body.getPos().y >= (Scene::Height() + 100)) deleteSelf();

	mirrored = body.getPos().x < player_position.x;

	body.applyLinearImpulse({ mirrored ? SLIME_WALK_POWER : -SLIME_WALK_POWER, 0 });
}

void Slime::draw() const
{
	Array<Vec2> vertices;

	for (const auto& b : aroundBodies) vertices << b.getPos();

	double alpha = 0.9;

	if (damaged) alpha = 0.5;

	Polygon(ChaikinSmooth(vertices)).calculateBuffer(outerRadius).draw({ 0.1, 0.1, 0.1, alpha });

	Vec2 center = body.getPos();

	Circle{ center + Vec2{ -10, -5 }, 3 }.draw(Palette::Red);
	Circle{ center + Vec2{  10, -5 }, 3 }.draw(Palette::Red);
}

void Slime::onDamaged(float amount)
{
	if (!damaged)
	{
		CharacterBase::onDamaged(amount);

		AudioAsset(U"Vaillant Damage").playOneShot();

		body.applyLinearImpulse({ (!mirrored ? SLIME_WALK_POWER : -SLIME_WALK_POWER) * 200, -SLIME_JUMP_POWER });

		SetTimeout([this] { damaged = false; }, 750ms);

		damaged = true;
	}
}

void Slime::destroy()
{
	if (!destroy_executed)
	{
		body.release();

		SetTimeout([this] { ObjectBase::destroy(); }, 1000ms);

		destroy_executed = true;
	}
}
