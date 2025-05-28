# include "Wall.hpp"

Wall::Wall(P2World& world, const Vec2& position) : ObjectBase(world, position)
{
	body = world.createRect(P2Static, position, SizeF{ 10, 1000 }); // 島袋 TODO: ステージごとに長さや見た目を変える
}
