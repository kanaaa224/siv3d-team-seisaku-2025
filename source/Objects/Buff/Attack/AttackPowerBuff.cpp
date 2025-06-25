#include "AttackPowerBuff.h"
//Player
#include "../../../Characters/Player.hpp"


AttackPowerBuff::AttackPowerBuff(P2World& world, const Vec2& position)
	: BuffBase(world, position)
{
	type = eBuffType::eAttackPower;

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(P2Dynamic, position, SizeF{ size.x, size.y }, P2Material{ .friction = 0.0 },P2Filter{});
	body.setFixedRotation(true);//当たり判定の回転を無くす

	assetName = U"AttackPowerBuff";
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
	if (Player* player = dynamic_cast<Player*>(&object)) {
		if (!doOncePara) {
			player->SetPlayerDamageUp(ATTACKPOWER_BUFF_PARAMETER);
			doOncePara = true;
		}
	}

	BuffBase::onHit(object);//親クラスのメゾットを呼び出す
}
