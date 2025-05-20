#include "BuffBase.hpp"

BuffBase::BuffBase(P2World& world, const Vec2& position) :
	ObjectBase(world, position)//リスポーン位置
{
	//velocity = Vec2(0.0, (-2 * GRAVITY));
}

BuffBase::~BuffBase()
{
}

void BuffBase::update()
{
	//重力
	//velocity.y += GRAVITY;

	//バフの動き
	drop_movement();

	//座標位置の更新
	//position += velocity * Scene::DeltaTime();

#ifdef DEBUG
	//仮地面
	/*if (position.y >= 400.0) {
		velocity.y = 0.0;
	}*/
#endif // DEBUG

}

void BuffBase::draw() const
{
#ifdef DEBUG
	//仮表示
	//RectF{ Arg::center(position.x,position.y),size.x,size.y }.draw();
#endif // DEBUG
}

void BuffBase::drop_movement()
{
	
}
