#include "Scarerun.hpp"
//画像の分割読み込み
#include "../../../Utils/CustomImageLoader.hpp"

Scarerun::Scarerun(P2World& world, const Vec2& position):
	EnemyBase(world, position)//初期位置
{
	type = eEnemyType::scarerun;

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(P2Dynamic, position, SizeF{ 50, 60 },P2Material{ .friction = 0.0 });
	body.setFixedRotation(true);//当たり判定の回転を無くす

	//画像を分割読み込み
	idle_img           = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//idle画像
	attackPosition_img = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//attackPosition画像(仮)
	attack_img         = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//attack画像(仮)
	getAttack_img      = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//getAttack画像(仮)
	die_img            = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//die画像(仮)
	now_texture = idle_img[0];//初期化用の画像
}

Scarerun::~Scarerun()
{
}

void Scarerun::update()
{
	EnemyBase::update();

	//視界内にプレイヤーがいる場合
	if (isPlayerInSight() == true && (nowState != DIE && nowState != ATTACK && getDamageFlg == false) || nowState == ATTACK_POSITION) {
		setEnemyState(ATTACK_POSITION);//攻撃姿勢状態へ
		if (nowStateTime >= attackPosition_img.size() * IMG_CHANGE_TIME) {//アニメーションを終えたら攻撃へ
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
}

void Scarerun::draw() const
{
	Vec2 size = Vec2(100, 100);
	now_texture.mirrored(img_flipFlg).resized(size).drawAt(body.getPos());
	body.drawFrame();
	drawHP();
#ifdef _DEBUG
	Print << U"Enemy_Scarerun_Velocity : " << body.getVelocity();
	Print << U"Enemy_Scarerun_SpawnPos : " << spawnPosition;
	Print << U"Enemy_Scarerun_Pos : " << body.getPos();
	Print << U"Enemy_Scarerun_NowState : " << nowState;
	Print << U"Enemy_Scarerun_OldState : " << oldState;
	Print << U"Enemy_Scarerun_StateNum : " << nowStateTime;
	Print << U"Enemy_Scarerun_HP : " << hp;
	Print << U"Enemy_Scarerun_FlipFlg : " << img_flipFlg;
#endif // DEBUG
}

void Scarerun::stateControl()
{
	switch (nowState)
	{
	case NONE:
		break;
	case IDLE:
		movement(70.0f);//左右移動（数値は移動する距離）
		break;
	case ATTACK_POSITION:
		//body.setVelocity(Vec2{ 0.0,body.getVelocity().y});
		doOncePosZero(Vec2{ 0.0,body.getVelocity().y });
		//ここに攻撃準備のSE
		break;
	case ATTACK:
		attackMove();
		//ここに攻撃のSE
		break;
	case GET_ATTACK:
		//ここにダメージを受けた時のエフェクト
		//アニメーションが終わったら次の状態へ遷移
		if (nowStateTime >= getAttack_img.size() * IMG_CHANGE_TIME) {
			getDamageFlg = false;
			setEnemyState(ATTACK_POSITION);
		}
		break;
	case DIE:
		//body.setVelocity(Vec2{ 0.0,body.getVelocity().y });
		doOncePosZero(Vec2{ 0.0,body.getVelocity().y });
		//ここに死亡時のエフェクト
		//ここに死亡時のSE
		break;
	default:
		break;
	}
}
