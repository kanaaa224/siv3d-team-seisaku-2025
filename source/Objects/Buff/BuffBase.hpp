#pragma once
#include "../ObjectBase.hpp"//親クラス

#define DEBUG
#define GRAVITY 10.0

class BuffBase : public ObjectBase
{
protected:
	Vec2 size;

public:
	BuffBase(P2World& world, const Vec2& position);
	virtual ~BuffBase();

	virtual void update() override;
	virtual void draw() const override;

	virtual void onHit(ObjectBase& object) override;

protected:
	//ドロップ時の動き
	void drop_movement();
};

