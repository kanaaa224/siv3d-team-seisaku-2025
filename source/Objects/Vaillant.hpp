# pragma once

# include "CharacterBase.hpp"

class Vaillant : public CharacterBase
{
public:
	Vaillant(P2World& world, const Vec2& position);
	~Vaillant() = default;

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object) override;

	void onDamaged(float amount) override;
	void onHealed (float amount) override;

private:
	Vec2 start_position;

	void initialize();
};
