# pragma once

# include "Base.hpp"

class StageBackground : public ObjectBase
{
public:
	StageBackground(P2World& world, const Vec2& position);
	~StageBackground() = default;

	void draw() const override;

	void setCameraPos(Vec2 pos) { camera_pos = pos; }

private:
	Vec2 camera_pos;
};
