#include "Ammo.hpp"

Ammo::Ammo(P2World& world, const Vec2& position, double setSpeed, bool setPlayerTargetFlg)
	: ObjectBase(world, position)
{
	size = Vec2{ 25.0,25.0 };

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(
		P2Dynamic,
		position,
		size,
		P2Material{ .friction = 0.0 },
		P2Filter{
			.categoryBits = CollisionCategory::Enemy,
			.maskBits = CollisionCategory::All
		}
	);

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
#ifdef _DEBUG
	//当たり判定表示
	body.drawFrame();
#endif // DEBUG
}

void Ammo::onHit(ObjectBase& object)
{
}

void Ammo::movement()
{

}

void Ammo::animation()
{
}
