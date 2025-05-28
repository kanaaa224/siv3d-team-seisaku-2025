# pragma once

# include "Base.hpp"

class HitBox : public ObjectBase
{
private:
	Vec2 playerPos;
	Vec2 playerVelocity;

	int playerState;
	bool flip_flg;

	static bool flg;

public:
	HitBox(P2World& world, const Vec2& position);
	~HitBox() = default;

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object) override;

	void setPlayerPos(Vec2 pos) { playerPos = pos; };
	void setPlayerState(int state) { playerState = state; };
	void setPlayerVelocity(Vec2 velocity) { playerVelocity = velocity; };
	void setPlayerflip(bool flg) { flip_flg = flg; };

private:
	void initialize();
};
