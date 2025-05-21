#include "AttackBuff.h"

AttackBuff::AttackBuff(P2World& world, const Vec2& position)
	: BuffBase(world, position)
{
	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(P2Dynamic, position, SizeF{ size.x, size.y }, P2Material{ .friction = 0.0 },P2Filter{});
	body.setFixedRotation(true);//当たり判定の回転を無くす
	
}

void AttackBuff::update()
{
}

void AttackBuff::draw() const
{
	__super::draw();
}
