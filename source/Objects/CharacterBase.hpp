# pragma once

# include "ObjectBase.hpp"

class CharacterBase : public ObjectBase {
protected:
	float hp, max_hp;

public:
	CharacterBase(const Vec2& start_position) : ObjectBase(start_position) {}

	float getHP()    const { return hp; }
	float getMaxHP() const { return max_hp; }
};
