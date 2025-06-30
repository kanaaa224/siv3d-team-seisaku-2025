#include "Scarerun.hpp"
//画像の分割読み込み
#include "../../../Utils/CustomImageLoader.hpp"
//Stage
#include "../../../Stages/Stage1.hpp"
//effect
#include "../Effect/CloudOfDustEffect.h"

Scarerun::Scarerun(P2World& world, const Vec2& position):
	EnemyBase(world, position)//初期位置
{
	type = eEnemyType::scarerun;
	body_size = Vec2{ 50, 80 };
	hpbar.setMaxHP(max_hp);//hpバーに最大HPを設定

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRectSensor(
		P2Dynamic,
		position,
		SizeF{ body_size.x, body_size.y },
		//P2Material{ .friction = 0.0 },
		P2Filter{
			.categoryBits = CollisionCategory::Enemy,
			.maskBits = CollisionCategory::All
		}
	);
	body.setFixedRotation(true);//当たり判定の回転を無くす

	//画像を分割読み込み
	idle_img           = LoadDivGraph(U"Scarerun Run", Size(150, 45));//idle画像
	attackPosition_img = LoadDivGraph(U"Scarerun Idle", Size(150, 45));//attackPosition画像(仮)
	attack_img         = LoadDivGraph(U"Scarerun Run", Size(150, 45));//attack画像
	getAttack_img      = LoadDivGraph(U"Scarerun GetDamage", Size(150, 45));//getAttack画像
	die_img            = LoadDivGraph(U"Scarerun Death", Size(150, 45));//die画像
	now_texture = idle_img[0];//初期化用の画像
	img_size = Vec2{ 150 * 2, 45 * 2 };
	drawExcMark_pos = Vec2{ 0,-75 };
}

Scarerun::~Scarerun()
{
}

void Scarerun::update()
{
	//視界内にプレイヤーがいる場合
	if (isPlayerInSight() == true && (nowState != DIE && nowState != ATTACK && getDamageFlg == false) || nowState == ATTACK_POSITION) {
		setEnemyState(ATTACK_POSITION);//攻撃姿勢状態へ
		if (nowImageNum == attackPosition_img.size()) {//アニメーションを終えたら攻撃へ
			setEnemyState(ATTACK);
		}
	}

	if (getDamageFlg == true && hp > 0) {
		setEnemyState(GET_ATTACK);
	}

	//hpが０になったら死亡
	if (hp <= 0) {
		setEnemyState(DIE);
	}

	//状態遷移
	stateControl();

	animation(
		Scene::DeltaTime(),
		SCARERUN_IMG_CT_IDLE,
		SCARERUN_IMG_CT_ATTACK_POSTION,
		SCARERUN_IMG_CT_ATTACK,
		SCARERUN_IMG_CT_GET_ATTACK,
		SCARERUN_IMG_CT_DIE);

	//親のメゾットを実行
	EnemyBase::update();

	if (pos.y >= Scene::Height() - 45) {//数字は画像サイズ
		body.setGravityScale(0.0);
		body.setVelocity(Vec2{ body.getVelocity().x,0.0 });
	}
	else {
		body.setGravityScale(GRAVITY);
	}

#ifdef _DEBUG
	if (KeyE.pressed() && Key3.pressed()) {
		Stage* stage = Stage::GetInstance();
		stage->createObject<CloudOfDustEffect>(pos + Vec2{0.0,30.0}, img_flipFlg);
	}
#endif // _DEBUG

}

void Scarerun::draw() const
{
	//画像の描画
	now_texture.mirrored(img_flipFlg).resized(img_size).drawAt(pos);
	
	//hpバーの描画
	hpbar.draw({ Vec2{(pos.x - body_size.x / 2 - 10),(pos.y - body_size.y / 2 - 10)},hp_imgSize });

	//デバック表示
#ifdef _DEBUG
	Print << U"Enemy_Scarerun_Velocity : " << body.getVelocity();
	Print << U"Enemy_Scarerun_SpawnPos : " << spawnPosition;
	Print << U"Enemy_Scarerun_Pos : " << body.getPos();
	Print << U"Enemy_Scarerun_NowState : " << nowState;
	Print << U"Enemy_Scarerun_OldState : " << oldState;
	Print << U"Enemy_Scarerun_StateNum : " << nowStateTime;
	Print << U"Enemy_Scarerun_HP : " << hp;
	Print << U"Enemy_Scarerun_FlipFlg : " << img_flipFlg;
	Print << U"Enemy_Scarerun_HP_Rate : " << hp_rate;

	//物理シュミレーション上の実態を描画
	body.drawFrame();
#endif // DEBUG
}

void Scarerun::stateControl()
{
	switch (nowState)
	{
	case NONE:
		break;
	case IDLE:
		movement(70.0f, eMovementDirection::X);//左右移動（数値は移動する距離）
		break;
	case ATTACK_POSITION:
		//body.setVelocity(Vec2{ 0.0,body.getVelocity().y});
		doOncePosZero(Vec2{ 0.0,body.getVelocity().y });
		spawnExclamationMarkEffect(*this);
		break;
	case ATTACK:
		attackMove();
		//４秒後にIDLEにする
		if (nowStateTime >= 4.0) {
			setEnemyState(IDLE);
		}

		//エフェクト生成
		if (nowImageNum % 4 == 0) {
			if (!doOnceSpawnCloudOfDustFlg) {
				Stage* stage = Stage::GetInstance();
				stage->createObject<CloudOfDustEffect>(pos + Vec2{ 0.0,32.0 }, img_flipFlg);
				doOnceSpawnCloudOfDustFlg = true;
			}
		}
		else {
			doOnceSpawnCloudOfDustFlg = false;
		}


		break;
	case GET_ATTACK:
		//ここにダメージを受けた時のエフェクト
		//アニメーションが終わったら次の状態へ遷移
		if (nowImageNum == getAttack_img.size()) {
			getDamageFlg = false;
			setEnemyState(ATTACK_POSITION);
		}
		break;
	case DIE:
		//body.setVelocity(Vec2{ 0.0,body.getVelocity().y });
		body.release();
		doOncePosZero(Vec2{ 0.0,body.getVelocity().y });
		//ここに死亡時のエフェクト
		//ここに死亡時のSE
		break;
	default:
		break;
	}
}

void Scarerun::spawnCloudOfDustEffect()
{

}
