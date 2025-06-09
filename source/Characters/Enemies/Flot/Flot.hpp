#pragma once

#include "../EnemyBase.hpp"

//画像切替速度
#define Flot_IMG_CT_IDLE           0.07
#define Flot_IMG_CT_ATTACK_POSTION 0.1
#define Flot_IMG_CT_ATTACK         0.1
#define Flot_IMG_CT_GET_ATTACK     0.05
#define Flot_IMG_CT_DIE            0.2

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

