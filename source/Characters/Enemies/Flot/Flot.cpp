#include "Flot.hpp"
//画像の分割読み込み
#include "../../../Utils/CustomImageLoader.hpp"

Flot::Flot(P2World& world, const Vec2& position):
	EnemyBase(world, position)//初期位置
{
	max_hp = 50;
	hp = max_hp;

	fireFlg = false;

	type = eEnemyType::flot;

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(
		P2Dynamic,
		position,
		SizeF{ 75, 60 },
		P2Material{ .density = 0.0, .friction = 0.0 },
		P2Filter{
			.categoryBits = CollisionCategory::Enemy,
			.maskBits = CollisionCategory::All
		}
	);
	body.setFixedRotation(true);//当たり判定の回転を無くす

	//画像を分割読み込み
	idle_img           = LoadDivGraph(U"Flot Idle",      Size(150, 45));//idle画像
	attackPosition_img = LoadDivGraph(U"Flot Idle",      Size(150, 45));//attackPosition画像
	attack_img         = LoadDivGraph(U"Flot Attack",    Size(150, 45));//attack画像
	getAttack_img      = LoadDivGraph(U"Flot GetDamage", Size(150, 45));//getAttack画像
	die_img            = LoadDivGraph(U"Flot Death",     Size(150, 45));//die画像
	now_texture = idle_img[0];//初期化用の画像
}

Flot::~Flot()
{
}

void Flot::update()
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

	animation(Scene::DeltaTime(), 0.07, 0.1, 0.1, 0.05, 0.2);

	//親のメゾットを実行
	EnemyBase::update();
#ifdef _DEBUG
	//玉発射のデバックキー　E+2
	if (KeyE.pressed() && Key2.pressed()) {
		spawnAmmo(1000, true);
	}
#endif // _DEBUG
}

void Flot::draw() const
{
	Vec2 size = Vec2(150 * 2, 45 * 2);
	now_texture.mirrored(img_flipFlg).resized(size).drawAt(pos + Vec2{-5,-15});
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
		//movement(20.0f, eMovementDirection::Y);//左右移動（数値は移動する距離）
		break;
	case ATTACK_POSITION:
		//movement(20.0f, eMovementDirection::Y);
		break;
	case ATTACK:
		//movement(20.0f, eMovementDirection::Y);
		//ここで玉を生成する
		fireAmmo();
		break;
	case GET_ATTACK:
		if (nowImageNum == getAttack_img.size()) {
			getDamageFlg = false;
			setEnemyState(ATTACK_POSITION);
		}
		break;
	case DIE:
		body.release();
		doOncePosZero(Vec2{ 0.0,body.getVelocity().y });
		break;
	default:
		break;
	}
}

void Flot::fireAmmo()
{
	if (nowImageNum == attack_img.size()) {
		fireFlg = true;
	}
	else {
		fireFlg = false;
	}

	if (fireFlg == true) {
		spawnAmmo(200, true);
	}
}
