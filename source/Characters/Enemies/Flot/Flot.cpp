#include "Flot.hpp"
//画像の分割読み込み
#include "../../../Utils/CustomImageLoader.hpp"

Flot::Flot(P2World& world, const Vec2& position):
	EnemyBase(world, position)//初期位置
{
	max_hp = 50;
	hp = max_hp;

	type = eEnemyType::flot;

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(
		P2Dynamic,
		position,
		SizeF{ 50, 60 },
		P2Material{ .friction = 0.0 },
		P2Filter{
			.categoryBits = CollisionCategory::Enemy,
			.maskBits = CollisionCategory::All
		}
	);
	body.setFixedRotation(true);//当たり判定の回転を無くす

	//画像を分割読み込み
	idle_img           = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//idle画像(仮)
	attackPosition_img = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//attackPosition画像(仮)
	attack_img         = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//attack画像(仮)
	getAttack_img      = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//getAttack画像(仮)
	die_img            = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//die画像(仮)
	now_texture = idle_img[0];//初期化用の画像
}

Flot::~Flot()
{
}

void Flot::update()
{
	//親のメゾットを実行
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

	if (KeyE.pressed() && Key2.pressed()) {
		spawnAmmo(10, false);
	}
}

void Flot::draw() const
{
	Vec2 size = Vec2(100, 100);
	now_texture.mirrored(img_flipFlg).resized(size).drawAt(body.getPos(),Palette::Red);
	body.drawFrame();
	drawHP();

#ifdef _DEBUG
	Print << U"Enemy_Flot_Velocity : " << body.getVelocity();
	Print << U"Enemy_Flot_SpawnPos : " << spawnPosition;
	Print << U"Enemy_Flot_Pos : " << body.getPos();
	Print << U"Enemy_Flot_NowState : " << nowState;
	Print << U"Enemy_Flot_OldState : " << oldState;
	Print << U"Enemy_Flot_StateNum : " << nowStateTime;
	Print << U"Enemy_Flot_HP : " << hp;
	Print << U"Enemy_Flot_FlipFlg : " << img_flipFlg;
#endif // DEBUG
}

void Flot::stateControl()
{
	switch (nowState)
	{
	case NONE:
		break;
	case IDLE:
		movement(20.0f, eMovementDirection::Y);//左右移動（数値は移動する距離）
		break;
	case ATTACK_POSITION:
		movement(20.0f, eMovementDirection::Y);
		break;
	case ATTACK:
		movement(20.0f, eMovementDirection::Y);
		//ここで玉を生成する
		break;
	case GET_ATTACK:
		if (nowStateTime >= getAttack_img.size() * IMG_CHANGE_TIME) {
			getDamageFlg = false;
			setEnemyState(ATTACK_POSITION);
		}
		break;
	case DIE:
		doOncePosZero(Vec2{ 0.0,body.getVelocity().y });
		break;
	default:
		break;
	}
}
