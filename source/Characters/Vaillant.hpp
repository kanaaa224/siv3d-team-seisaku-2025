# pragma once

# include "Base.hpp"
# include "../Effects/SpriteAnimator.hpp"

enum class VaillantState {
	Idle,
	Walk,
	Death,
	Destroy
};

class Vaillant : public CharacterBase
{
public:
	Vaillant(P2World& world, const Vec2& position);
	~Vaillant() = default;

	void update() override;
	void draw() const override;

	void destroy() override;
	void die() override;

	void onHit(ObjectBase& object) override;
	void onDamaged(float amount) override;
	
	void setPlayerPosition(Vec2 pos) { player_position = pos; }

private:
	Vec2 position;

	Vec2 start_position;
	Vec2 player_position;
	
	SizeF size;
	
	double frameTime;
	
	VaillantState state;
	
	bool mirrored;
	bool attack_started;
	bool die_executed;
	bool destroy_executed;

	SpriteAnimator spriteAnimator;

	void initialize();
};
