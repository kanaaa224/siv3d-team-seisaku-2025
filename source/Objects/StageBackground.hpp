# pragma once

# include "Base.hpp"

class StageBackground : public ObjectBase
{
private:
	Vec2 pos_1st;
	Vec2 pos_2nd;

	Vec2 camera_pos;
	Vec2 cameraTopLeft;
	Vec2 playerVelocity;

public:
	StageBackground(P2World& world, const Vec2& position);
	~StageBackground() = default;

	void update() override;
	void draw() const override;

	void setCameraPos(Vec2 pos);
	void setPlayerVelocity(Vec2 velocity);
};
