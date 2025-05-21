#pragma once

#include "../BuffBase.hpp"

class AttackBuff : public BuffBase
{
public:
	AttackBuff(P2World& world, const Vec2& position);

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object) override {};
};

