# pragma once

# include "ObjectBase.hpp"

class Ground : public ObjectBase
{
public:
	Ground(P2World& world, const Vec2& position);
	~Ground() = default;
};
