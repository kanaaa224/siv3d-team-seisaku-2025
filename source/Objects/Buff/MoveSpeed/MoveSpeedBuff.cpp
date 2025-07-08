#include "MoveSpeedBuff.h"
//Player
#include "../../../Characters/Player.hpp"

MoveSpeedBuff::MoveSpeedBuff(P2World& world, const Vec2& position)
	: BuffBase(world, position)
{
	type = eBuffType::eMoveSpeed;

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(P2Dynamic, position, SizeF{ size.x, size.y }, P2Material{ .friction = 0.0 }, P2Filter{});
	body.setFixedRotation(true);//当たり判定の回転を無くす

	assetName = U"MoveSpeedBuff";
}

void MoveSpeedBuff::update()
{
}

void MoveSpeedBuff::draw() const
{
	BuffBase::draw();
}

void MoveSpeedBuff::onHit(ObjectBase& object, const P2Collision& collision)
{
	if (Player* player = dynamic_cast<Player*>(&object)) {
		player->SetPlayerSpeed(MOVESPEED_BUFF_PARAMETER);
	}

	BuffBase::onHit(object, collision);//親クラスのメゾットを呼び出す
}
