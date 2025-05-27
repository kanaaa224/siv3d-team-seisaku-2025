# pragma once

# include "ObjectBase.hpp"

class HitBox : public ObjectBase
{
private:
	Vec2 playerPos;

	int playerState;

public:
	HitBox(P2World& world, const Vec2& position);
	~HitBox();

	void update() override;
	void draw() const override;

	void setPlayerPos(Vec2 pos) { playerPos = pos; };
	void setPlayerState(int state) { playerState = state; };

	void onHit(ObjectBase& object) override;

private:
	void initialize();
	void finalize();
};
