#include "EnemyBase.hpp"
#include "../../Stages/Stage1.hpp"
#include "../Buff/Attack/AttackBuff.h"
#include "../Enemy/Scarerun/Scarerun.hpp"

EnemyBase::EnemyBase(P2World& world, const Vec2& position) :
	CharacterBase(world, position)//初期位置
{
	hp = 100.0f;//体力
	setEnemyState(IDLE);//ステータス
	body.setGravityScale(GRAVITY);//重力
	spawnPosition = position;//スポーン位置
	nowWorld = &world;//現在の物理シュミレーション

	spawnBuffFlg = false;
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::update()
{
	//ダメージストップ処理
	//Flgが経って何秒経過しているか
	if (damageStopFlg == true) {
		damageStop_ct += Scene::DeltaTime();
		//設定した時間になったらFlgをFalseにする
		if (damageStop_ct >= DAMAGE_STOP_TIME) {
			damageStopFlg = false;
			damageStop_ct = 0;
		}
	}

	animation(Scene::DeltaTime());

	if (nowState == DIE && nowStateTime >= 3.0f && spawnBuffFlg == false) {
		spawnBuff();
		spawnBuffFlg = true;
	}

#ifdef DEBUG
	if (KeyE.pressed() && Key0.pressed()) {//(E + 0)でhpを0にする
		hp = 0;
	}
	if (KeyE.pressed() && Key1.pressed()) {//(E + 1)でhpを10減らす
		getDamage(10);
	}
#endif // DEBUG
}

void EnemyBase::draw() const
{


#ifdef DEBUG
	//画像ができるまでの仮表示
	//Rect(position.x, position.y, size.x, size.y).draw(Palette::Red);

	
#endif // DEBUG
}

void EnemyBase::animation(float delta_second)
{
	switch (nowState)
	{
	case IDLE:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
			nowStateTime = 0;
			nowImageNum = 0;
			imageChangeTime = 0;
		}
		//現在のステートになって何秒経過しているか加算
		nowStateTime += delta_second;
		//現在のステートを更新
		setEnemyState(IDLE);
		//現在の画像が何秒経過したか
		imageChangeTime += delta_second;

		//コンテナの最後の要素数になったら最初の画像の要素数にする
		if (nowImageNum >= idle_img.size()) {
			nowImageNum = 0;
		}

		//画像切り替え
		if (imageChangeTime >= IMG_CHANGE_TIME && nowImageNum <= idle_img.size()) {
			now_texture = idle_img[nowImageNum];//画像を更新
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	case ATTACK_POSITION:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
			nowStateTime = 0;
			nowImageNum = 0;
			imageChangeTime = 0;
		}
		//現在のステートになって何秒経過しているか加算
		nowStateTime += delta_second;
		//現在のステートを更新
		setEnemyState(ATTACK_POSITION);
		//現在の画像が何秒経過したか
		imageChangeTime += delta_second;

		//コンテナの最後の要素数になったら最初の画像の要素数にする
		if (nowImageNum >= attackPosition_img.size()) {
			nowImageNum = 0;
		}

		//画像切り替え
		if (imageChangeTime >= IMG_CHANGE_TIME && nowImageNum <= attackPosition_img.size()) {
			now_texture = attackPosition_img[nowImageNum];
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	case ATTACK:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
			nowStateTime = 0;
			nowImageNum = 0;
			imageChangeTime = 0;
		}
		//現在のステートになって何秒経過しているか加算
		nowStateTime += delta_second;
		//現在のステートを更新
		setEnemyState(ATTACK);
		//現在の画像が何秒経過したか
		imageChangeTime += delta_second;

		//コンテナの最後の要素数になったら最初の画像の要素数にする
		if (nowImageNum >= attack_img.size()) {
			nowImageNum = 0;
		}

		//画像切り替え
		if (imageChangeTime >= IMG_CHANGE_TIME && nowImageNum <= attack_img.size()) {
			now_texture = attack_img[nowImageNum];
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	case GET_ATTACK:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
			nowStateTime = 0;
			nowImageNum = 0;
			imageChangeTime = 0;
		}
		//現在のステートになって何秒経過しているか加算
		nowStateTime += delta_second;
		//現在のステートを更新
		setEnemyState(GET_ATTACK);
		//現在の画像が何秒経過したか
		imageChangeTime += delta_second;

		//コンテナの最後の要素数になったら最初の画像の要素数にする
		if (nowImageNum >= getAttack_img.size()) {
			nowImageNum = 0;
		}

		//画像切り替え
		if (imageChangeTime >= IMG_CHANGE_TIME && nowImageNum <= getAttack_img.size()) {
			now_texture = getAttack_img[nowImageNum];
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	case DIE:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
			nowStateTime = 0;
			nowImageNum = 0;
			imageChangeTime = 0;
		}
		//現在のステートになって何秒経過しているか加算
		nowStateTime += delta_second;
		//現在のステートを更新
		setEnemyState(DIE);
		//現在の画像が何秒経過したか
		imageChangeTime += delta_second;

		//コンテナの最後の要素数になったら最初の画像の要素数にする
		if (nowImageNum >= die_img.size()) {
			nowImageNum = 0;
		}

		//画像切り替え
		if (imageChangeTime >= IMG_CHANGE_TIME && nowImageNum <= die_img.size()) {
			now_texture = die_img[nowImageNum];
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	default:
		break;
	}
}

void EnemyBase::movement(float distance)
{
	if (spawnPosition.x <= body.getPos().x) {
		body.setVelocity(Vec2{ -MOVE_SPEED,body.getVelocity().y});
		img_flipFlg = true;
	}
	if (spawnPosition.x - distance >= body.getPos().x) {
		body.setVelocity(Vec2{ MOVE_SPEED,body.getVelocity().y });
		img_flipFlg = false;
	}
}

void EnemyBase::drawHP() const
{
	//現在のHP割合
	float hp_rate = hp / max_hp;
	float hp_x_size = HP_X_MAXSIZE * hp_rate;

	//HPバーをBoxで描画
	
}

void EnemyBase::getDamage(float damage)
{
	//設定した時間経過後またダメージを受ける
	if (damageStopFlg == false) {
		hp -= damage;
		damageStopFlg = true;
	}

	//HPが０以下なら０にする
	if (hp <= 0) {
		hp = 0;
	}
}

void EnemyBase::spawnBuff()
{
	Stage* stage = Stage1::GetInstance();

	stage->addObject<AttackBuff>(body.getPos());
}
