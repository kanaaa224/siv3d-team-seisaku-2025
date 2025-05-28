# pragma once

# include "Base.hpp"

class Ground : public ObjectBase
{
public:
	Ground(P2World& world, const Vec2& position);
	~Ground() = default;
};
