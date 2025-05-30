# pragma once

# include "Base.hpp"

class StageBackground : public ObjectBase
{
private:
	Vec2 left_position;
	Vec2 right_position;

	Vec2 camera_pos;

public:
	StageBackground(P2World& world, const Vec2& position);
	~StageBackground() = default;

	void update() override;
	void draw() const override;

	void setCameraPos(Vec2 pos);
};
