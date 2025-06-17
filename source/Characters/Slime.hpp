/* Copied from "https://github.com/kanaaa224/siv3d-2d-scroll" */

# pragma once

# include "Base.hpp"

class Slime : public CharacterBase
{
public:
	Slime(P2World& world, const Vec2& position = { 0, 0 });
	~Slime() = default;

	void update() override;
	void draw() const override;

	void setPlayerPosition(Vec2 pos = { 0, 0 }) { player_position = pos; }

private:
	Array<P2Body> aroundBodies;
	Array<P2DistanceJoint> joints;

	double centerRadius;
	double outerRadius;

	bool mirrored;

	Vec2 player_position;
};
