#pragma once
#include "../Base.hpp"//親クラス

#define GRAVITY 10.0
#define IMG_SIZE_X 19
#define IMG_SIZE_Y 23

class BuffBase : public ObjectBase
{
protected:
	Vec2 size;
	String assetName;

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

