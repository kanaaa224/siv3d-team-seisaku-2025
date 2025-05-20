#pragma once
#include "../ObjectBase.hpp"//親クラス

#define DEBUG
#define GRAVITY 10.0

class BuffBase : public ObjectBase
{
public:
	BuffBase(P2World& world, const Vec2& position);
	virtual ~BuffBase();

	virtual void update() override;
	virtual void draw() const override;

protected:
	//ドロップ時の動き
	void drop_movement();
};

