# pragma once

# include "ObjectBase.hpp"

class Wall : public ObjectBase
{
public:
	Wall(P2World& world, const Vec2& position);
	~Wall();
};
