#include "Ammo.hpp"

Ammo::Ammo(P2World& world, const Vec2& position, double setSpeed, bool setPlayerTargetFlg)
	: ObjectBase(world, position)
{
	speed = setSpeed;//発射速度
	playerTargetFlg = setPlayerTargetFlg;//プレイヤーに向かって発射するか
}

Ammo::~Ammo()
{
}

void Ammo::update()
{
}

void Ammo::draw() const
{
}

void Ammo::onHit(ObjectBase& object)
{
}

void Ammo::movement()
{
}
