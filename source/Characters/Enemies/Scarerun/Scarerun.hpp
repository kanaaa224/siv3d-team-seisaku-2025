#pragma once

#include "../EnemyBase.hpp"

//画像切替速度
#define SCARERUN_IMG_CT_IDLE           0.13
#define SCARERUN_IMG_CT_ATTACK_POSTION 0.1
#define SCARERUN_IMG_CT_ATTACK         0.1
#define SCARERUN_IMG_CT_GET_ATTACK     0.05
#define SCARERUN_IMG_CT_DIE            0.2

class Scarerun : public EnemyBase
{
public:
	Scarerun(P2World& world, const Vec2& position);
	~Scarerun();

	void update() override;
	void draw() const override;

	//void onHit(ObjectBase& object) override;

private:
	void stateControl() override;

	//攻撃時の砂埃エフェクトを再生する
	void spawnCloudOfDustEffect();
	bool doOnceSpawnCloudOfDustFlg;
};

