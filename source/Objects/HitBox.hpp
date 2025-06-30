# pragma once

# include "ObjectBase.hpp"

class HitBox : public ObjectBase
{
public:
	HitBox(P2World& world, const Vec2& position, ObjectBase& owner);
	~HitBox() = default;

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object) override;

private:
	ObjectBase* owner = nullptr;
};
