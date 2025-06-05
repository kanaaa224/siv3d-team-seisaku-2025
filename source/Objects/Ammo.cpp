#include "Ammo.hpp"
//Player
#include "../Characters/Player.hpp"
//Ground
#include "Ground.hpp"

Ammo::Ammo(P2World& world, const Vec2& position, double setSpeed, bool setPlayerTargetFlg, Vec2 pPos)
	: ObjectBase(world, position)
{
	size = Vec2{ 25.0,25.0 };
	playerPos = pPos;

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
	movement();

	if (body) {
		pos = body.getPos();
	}
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
	//プレイヤーに当たった時
	if (Player* p = dynamic_cast<Player*>(&object)) {
		deleteSelf();
	}
	//地面に当たった時
	if (Ground* g = dynamic_cast<Ground*>(&object)) {
		deleteSelf();
	}
}

void Ammo::movement()
{
	if (playerTargetFlg == true) {
		//プレイヤーまでの距離を計算
		Vec2 playerDist = playerPos - body.getPos();
		double length = playerDist.length();

		if (length > 0) {
			body.setVelocity(Vec2{ playerDist.x / length * (30),playerDist.y / length * (30) });

			if (body.getVelocity().x < 0) {
				img_flipFlg = true;
			}
			else if(body.getVelocity().x > 0){
				img_flipFlg = false;
			}
		}
	}
	else {

	}
}

void Ammo::animation()
{
}
