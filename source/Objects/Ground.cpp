# include "Ground.hpp"

Ground::Ground(P2World& world, const Vec2& position) : ObjectBase(world, position)
{
	body = world.createRect(P2Static, position, SizeF{ 15000, 10 }, P2Material{ .restitution = 0.0, }); // 島袋 TODO: ステージごとに長さや見た目を変える
}
