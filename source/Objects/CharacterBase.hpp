﻿# pragma once

# include "ObjectBase.hpp"

class CharacterBase : public ObjectBase
{
protected:
	float hp, max_hp;

public:
	CharacterBase(P2World& world, const Vec2& position) : ObjectBase(world, position) {}

	float getHP()    const { return hp; }
	float getMaxHP() const { return max_hp; }

	virtual void onHit(CharacterBase& object) {}
};
