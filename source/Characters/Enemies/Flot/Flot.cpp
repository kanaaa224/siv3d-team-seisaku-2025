#include "Flot.hpp"
//画像の分割読み込み
#include "../../../Utils/CustomImageLoader.hpp"

Flot::Flot(P2World& world, const Vec2& position):
	EnemyBase(world, position)//初期位置
{
	max_hp = 60;
	hp = max_hp;

	fireFlg = false;

	type = eEnemyType::flot;
	body_size = Vec2{ 75, 60 };

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRectSensor(
		P2Dynamic,
		position,
		SizeF{ body_size.x, body_size.y },
		//P2Material{ .density = 0.0, .friction = 0.0 },
		P2Filter{
			.categoryBits = CollisionCategory::Enemy,
			.maskBits = CollisionCategory::All
		}
	);
	body.setFixedRotation(true);//当たり判定の回転を無くす
	body.setGravityScale(0.0);

	//画像を分割読み込み
	idle_img           = LoadDivGraph(U"Flot Idle",      Size(150, 45));//idle画像
	attackPosition_img = LoadDivGraph(U"Flot Idle",      Size(150, 45));//attackPosition画像
	attack_img         = LoadDivGraph(U"Flot Attack",    Size(150, 45));//attack画像
	getAttack_img      = LoadDivGraph(U"Flot GetDamage", Size(150, 45));//getAttack画像
	die_img            = LoadDivGraph(U"Flot Death",     Size(150, 45));//die画像
	now_texture = idle_img[0];//初期化用の画像
	img_size = Vec2{ 150 * 2, 45 * 2 };
}

Flot::~Flot()
{
}

void Flot::update()
{
	//視界内にプレイヤーがいる場合
	if (isPlayerInSight() == true && (nowState != DIE && nowState != ATTACK && getDamageFlg == false) || nowState == ATTACK_POSITION) {
		setEnemyState(ATTACK_POSITION);//攻撃姿勢状態へ
		if (moveAwayEndFlg) {//プレイヤーから一定距離離れたら
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

	if (nowState == DIE) {
		if (pos.y <= Scene::Height() + 5) {
			pos.y += 2.0;
		}
		
	}

	//状態遷移
	stateControl();
	//アニメーション
	animation(
		Scene::DeltaTime(),
		Flot_IMG_CT_IDLE,
		Flot_IMG_CT_ATTACK_POSTION,
		Flot_IMG_CT_ATTACK,
		Flot_IMG_CT_GET_ATTACK,
		Flot_IMG_CT_DIE);

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
	now_texture.mirrored(img_flipFlg).resized(img_size).drawAt(pos + Vec2{-5,-15});
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
		
		break;
	case ATTACK_POSITION:
		//プレイヤーから距離を取る
		MoveAwayFromPlayer(AWAY_PLAYER, APPROACDDISTANCE);
		break;
	case ATTACK:
		if (playerPos.x - pos.x > 0) {//プレイヤーが左
			img_flipFlg = false;
		}
		else {//プレイヤーが右
			img_flipFlg = true;
		}

		fireAmmo();//玉の生成
		if (fireFlg) {
			setEnemyState(ATTACK_POSITION);
			moveAwayEndFlg = false;
		}
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
		spawnAmmo(AMMO_SPEED, true);
	}
}

void Flot::MoveAwayFromPlayer(double awayDistance, double approachDistance)
{
	//初期化
	if (!MoveAwayInitFlg) {
		moveAwayEndFlg = false;
		MoveAwayInitFlg = true;
	}

	if ((playerPos - pos).length() <= awayDistance || (playerPos - pos).length() >= approachDistance) {
		if (playerPos.x - pos.x > 0) {//プレイヤーが左
			movePoint = Vec2{ playerPos.x - awayDistance,pos.y };
			img_flipFlg = false;
		}
		else {//プレイヤーが右
			movePoint = Vec2{ playerPos.x + awayDistance,pos.y };
			img_flipFlg = true;
		}
	}
	else {
		movePoint = Vec2{ pos.x,pos.y };
	}

	Vec2 dir = movePoint - pos;
	double distance = dir.length();

	body.setVelocity((distance > 0.0) ? (dir / distance) * FLY_MOVE_SPEED : Vec2{ 0, 0 });

	if (distance <= 10.0) {
		moveAwayEndFlg = true;
		MoveAwayInitFlg = false;
		body.setVelocity(Vec2{ 0.0,0.0 });
	}
}
