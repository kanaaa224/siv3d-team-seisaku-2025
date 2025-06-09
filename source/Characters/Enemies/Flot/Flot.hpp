#pragma once

#include "../EnemyBase.hpp"

//画像切替速度
#define Flot_IMG_CT_IDLE           0.07
#define Flot_IMG_CT_ATTACK_POSTION 0.1
#define Flot_IMG_CT_ATTACK         0.1
#define Flot_IMG_CT_GET_ATTACK     0.05
#define Flot_IMG_CT_DIE            0.2

//移動速度
#define FLY_MOVE_SPEED 100.0
#define AMMO_SPEED 200.0

#define AWAY_PLAYER 450.0

class Flot : public EnemyBase
{
public:
	Flot(P2World& world, const Vec2& position);
	~Flot();

	void update() override;
	void draw() const override;

private:
	void stateControl() override;

	//玉の生成
	void fireAmmo();
	bool fireFlg;//玉を生成するか？
	double fire_coolTime;//次の玉の発射までのクールタイム

	//プレイヤーから距離を取る
	void MoveAwayFromPlayer(double awayDistance);
	Vec2 movePoint;//移動する場所
	bool MoveAwayInitFlg;//moveAwayFromPlayerの中で初期化
	bool moveAwayEndFlg;//移動する場所に着いたか
};

