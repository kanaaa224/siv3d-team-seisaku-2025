#include "AttackPowerBuff.h"


AttackPowerBuff::AttackPowerBuff(P2World& world, const Vec2& position)
	: BuffBase(world, position)
{
	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(P2Dynamic, position, SizeF{ size.x, size.y }, P2Material{ .friction = 0.0 },P2Filter{});
	body.setFixedRotation(true);//当たり判定の回転を無くす
}

void AttackPowerBuff::update()
{
}

void AttackPowerBuff::draw() const
{
	BuffBase::draw();
}

void AttackPowerBuff::onHit(ObjectBase& object)
{
	BuffBase::onHit(object);//親クラスのメゾットを呼び出す
}
