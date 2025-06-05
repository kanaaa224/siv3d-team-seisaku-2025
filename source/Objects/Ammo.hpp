#pragma once

//親クラス
#include "Base.hpp"

#define _LIFE_TIME_ 3.0

class Ammo : public ObjectBase
{
private:
	double lifeTime;

	Vec2 size;
	Vec2 playerPos;
	Vec2 pos;

	bool img_flipFlg;

	double nowStateTime;

	double speed;        //玉の発射速度
	bool playerTargetFlg;//プレイヤーに向かって玉が飛ぶか

	Vec2 moveDirection; // ← グローバル or クラスメンバとして保持
	bool initialized = false;

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
};

