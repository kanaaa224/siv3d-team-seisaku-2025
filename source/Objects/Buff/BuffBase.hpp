#pragma once
#include "../ObjectBase.hpp"

class BuffBase : public ObjectBase
{
public:
	BuffBase(const Vec2& start_position);
	virtual ~BuffBase();

	virtual void update() override;
	virtual void draw() const override;
};

