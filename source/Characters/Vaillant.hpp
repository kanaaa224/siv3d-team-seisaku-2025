# pragma once

# include "Base.hpp"

class Vaillant : public CharacterBase
{
public:
	Vaillant(P2World& world, const Vec2& position);
	~Vaillant() = default;

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object) override;
	
	void setPlayerPosition(Vec2 position) { player_position = position; }

private:
	Vec2 start_position;
	
	SizeF size;
	
	double animationTime;
	
	int state;
	
	bool mirrored;
	
	Vec2 player_position;
	
	bool attack_started;

	void initialize();
};
