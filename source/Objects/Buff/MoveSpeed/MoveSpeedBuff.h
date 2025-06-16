#pragma once

#include "../BuffBase.hpp"

class MoveSpeedBuff : public BuffBase
{
public:
	MoveSpeedBuff(P2World& world, const Vec2& position);

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object) override;
};

