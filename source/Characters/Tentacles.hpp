# pragma once

# include "CharacterBase.hpp"

# define TENTACLES_SIZE { 100, 100 }
# define TENTACLES_MAX_HP 100

class Tentacles : public CharacterBase
{
public:
	Tentacles(P2World& world, const Vec2& position = { 0, 0 });
	~Tentacles() = default;

	void update() override;
	void draw() const override;

	void destroy() override;
	void die() override;

	void onHit(ObjectBase& object) override;
	void onDamaged(float amount) override;
	
	void setPlayerPosition(Vec2 pos = { 0, 0 }) { player_position = pos; }

private:
	Vec2 position;
	Vec2 start_position;
	Vec2 player_position;
	
	SizeF size;
	
	double frameTime;
	
	bool damaged;
	bool die_executed;
	bool destroy_executed;
};
