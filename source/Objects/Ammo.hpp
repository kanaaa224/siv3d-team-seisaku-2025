#pragma once

//親クラス
#include "Base.hpp"

class Ammo : public ObjectBase
{
private:
	double speed;        //玉の発射速度
	bool playerTargetFlg;//プレイヤーに向かって玉が飛ぶか

public:
	Ammo(P2World& world, const Vec2& position, double setSpeed, bool setPlayerTargetFlg);
	~Ammo() override;

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object) override;

private:
	void movement();
};

