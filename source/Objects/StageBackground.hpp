# pragma once

# include "Base.hpp"

class StageBackground : public ObjectBase
{
private:
	Vec2 texture_1_pos;
	Vec2 texture_2_pos;

	Vec2 camera_pos;

public:
	StageBackground(P2World& world, const Vec2& position);
	~StageBackground() = default;

	void update() override;
	void draw() const override;

	void setCameraPos(Vec2 pos) { camera_pos = pos; }
};
