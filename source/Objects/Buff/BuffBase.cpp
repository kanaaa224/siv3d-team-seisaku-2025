#include "BuffBase.hpp"
//Player
#include "../../Characters/Player.hpp"
//Stage
#include "../../Stages/Stage1.hpp"
//effect
#include "../../Characters/Enemies/Effect/BuffSpawnEffect.h"

BuffBase::BuffBase(P2World& world, const Vec2& position) :
	ObjectBase(world, position)//リスポーン位置
{
	size = Vec2{ 25.0,25.0 };

	//エフェクト
	Stage* stage = Stage::GetInstance();
	stage->createObject<BuffSpawnEffect>(position, *this, U"");
}

BuffBase::~BuffBase()
{
}

void BuffBase::update()
{

#ifdef _DEBUG

#endif // DEBUG

}

void BuffBase::draw() const
{
	TextureAsset(assetName).resized(Vec2{ IMG_SIZE_X * 1.5,IMG_SIZE_Y * 1.5 }).drawAt(body.getPos());

#ifdef _DEBUG
	//仮表示
	//RectF{ Arg::center(body.getPos().x,body.getPos().y),size.x,size.y}.draw();
#endif // DEBUG
}

void BuffBase::onHit(ObjectBase& object, const P2Collision& collision)
{
	if (Player* player = dynamic_cast<Player*>(&object)) {
		AudioAsset(U"buff_get_SE").playOneShot();//SE再生
		deleteSelf();//自分人身を削除
	}
}

void BuffBase::drop_movement()
{
	
}
