#pragma once

#include "../EnemyBase.hpp"

class Scarerun : public EnemyBase
{
public:
	Scarerun(P2World& world, const Vec2& position);
	~Scarerun();

	void update() override;
	void draw() const override;

	//void onHit(ObjectBase& object) override;

private:
	void stateControl() override;
};

