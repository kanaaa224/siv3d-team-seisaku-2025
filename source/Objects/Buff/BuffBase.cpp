#include "BuffBase.hpp"

BuffBase::BuffBase(P2World& world, const Vec2& position) :
	ObjectBase(world, position)//リスポーン位置
{
	size = Vec2{ 25.0,25.0 };
}

BuffBase::~BuffBase()
{
}

void BuffBase::update()
{

#ifdef DEBUG

#endif // DEBUG

}

void BuffBase::draw() const
{
#ifdef DEBUG
	//仮表示
	RectF{ Arg::center(body.getPos().x,body.getPos().y),size.x,size.y}.draw();
#endif // DEBUG
}

void BuffBase::onHit(ObjectBase& object)
{
}

void BuffBase::drop_movement()
{
	
}
