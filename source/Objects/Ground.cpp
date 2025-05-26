# include "Ground.hpp"

Ground::Ground(P2World& world, const Vec2& position) : ObjectBase(world, position)
{
	body = world.createRect(P2Static, position, SizeF{ 10000, 10 }); // 島袋 TODO: ステージごとに長さや見た目を変える
}

Ground::~Ground()
{

}
