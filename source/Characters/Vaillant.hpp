# pragma once

# include "Base.hpp"
# include "../Effects/SpriteAnimator.hpp"

# define VAILLANT_SIZE { 203, 233 }
# define VAILLANT_MAX_HP 100
# define VAILLANT_WALK_POWER 7.5
# define VAILLANT_JUMP_POWER 5000

enum class VaillantState {
	Idle,
	Walk,
	Death,
	Destroy
};

class Vaillant : public CharacterBase
{
public:
	Vaillant(P2World& world, const Vec2& position = { 0, 0 });
	~Vaillant() = default;

	void update() override;
	void draw() const override;

	void destroy() override;
	void die() override;

	void onHit(ObjectBase& object) override;
	void onDamaged(float amount) override;
	
	void setPlayerPosition(Vec2 pos = { 0, 0 }) { player_position = pos; }

	VaillantState getState() { return state; }

private:
	Vec2 position;

	Vec2 start_position;
	Vec2 player_position;
	
	SizeF size;
	
	double frameTime;
	
	VaillantState state;

	bool jumped;
	bool roaming_flipped;
	bool mirrored;
	bool attack_started;
	bool die_executed;
	bool destroy_executed;

	SpriteAnimator spriteAnimator;

	bool damaged;

	void initialize();
};
