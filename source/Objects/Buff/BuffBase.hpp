#pragma once
#include "../Base.hpp"//親クラス

#define GRAVITY 10.0
#define IMG_SIZE_X 19
#define IMG_SIZE_Y 23

//バフの上昇値
#define ATTACKPOWER_BUFF_PARAMETER 10  //攻撃力
#define MOVESPEED_BUFF_PARAMETER   30 //移動速度

enum eBuffType
{
	eAttackPower,
	eMoveSpeed
};

class BuffBase : public ObjectBase
{
protected:
	eBuffType type;

	Vec2 size;
	String assetName;

	bool doOncePara;

public:
	BuffBase(P2World& world, const Vec2& position);
	virtual ~BuffBase();

	virtual void update() override;
	virtual void draw() const override;

	virtual void onHit(ObjectBase& object) override;

	eBuffType getBuffType() { return type; };

protected:
	//ドロップ時の動き
	void drop_movement();
};

