#pragma once

//親クラス
#include "Base.hpp"

enum eAmmoState
{
	eA_NONE,
	eA_MOVE,
	eA_HIT,
	eA_DIE
};

class Ammo : public ObjectBase
{
private:
	Vec2 size;
	Vec2 playerPos;
	Vec2 pos;

	bool img_flipFlg;

	eAmmoState nowState, oldState;
	double nowStateTime;

	double speed;        //玉の発射速度
	bool playerTargetFlg;//プレイヤーに向かって玉が飛ぶか

public:
	Ammo(P2World& world, const Vec2& position, double setSpeed, bool setPlayerTargetFlg, Vec2 pPos);
	~Ammo() override;

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object) override;

private:
	//移動処理
	void movement();
	//アニメーション
	void animation();
	
	void setState(eAmmoState state)
	{
		if (oldState != nowState) {
			oldState = nowState;
			nowState = state;

			nowStateTime = 0.0;
		}
	};
};

