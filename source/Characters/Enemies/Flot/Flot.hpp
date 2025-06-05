#pragma once

#include "../EnemyBase.hpp"

class Flot : public EnemyBase
{
public:
	Flot(P2World& world, const Vec2& position);
	~Flot();

	void update() override;
	void draw() const override;

private:
	void stateControl() override;

	void fireAmmo();
	bool fireFlg;
	double fire_coolTime;
};

