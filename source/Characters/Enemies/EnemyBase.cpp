#include "EnemyBase.hpp"

//Player
#include "../Player.hpp"
//Stage
#include "../../Stages/1.hpp"
//Buff
#include "../../Objects/Buff/Attack/AttackPowerBuff.h"
#include "../../Objects/Buff/MoveSpeed/MoveSpeedBuff.h"
//Ammo
#include "../../Objects/Ammo.hpp"
//effect
#include "Effect/ExclamationMarkEffect.h"

EnemyBase::EnemyBase(P2World& world, const Vec2& position) :
	CharacterBase(world, position)//初期位置
{
	max_hp = 100.0f;//最大体力の設定
	hp = max_hp;//体力
	hp_rate = hp / max_hp;
	setEnemyState(IDLE);//ステータス
	body.setGravityScale(GRAVITY);//重力
	spawnPosition = position;//スポーン位置
	nowWorld = &world;//現在の物理シュミレーション

	spawnBuffFlg = false;//バフがスポーンしたか
	playerFoundFlg = false;//プレイヤーを発見したか

	hp_frame_img = TextureAsset(U"HP_frame");
	hp_imgSize = Vec2(70, 7);//Hp画像サイズ
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
			damageStop_ct = 0.0;
		}
	}

	if (body) {
		pos = body.getPos();
	}

	//死亡時にSEを再生
	if (nowState == DIE && !doOnceDieSE) {
		AudioAsset(U"Enemy_dies_SE").playOneShot();
		doOnceDieSE = true;
	}

	//Buffの生成
	if (nowState == DIE && nowImageNum == die_img.size()) {
		spawnBuff();
		spawnBuffFlg = true;
	}

	//Buff生成時に自分自身の削除
	if (spawnBuffFlg == true) {
		die();
	}

	//ステージの左端の外側にいかないようにする処理
	if (body.getPos().x <= 0) {
		body.setVelocity(Vec2{ - body.getVelocity().x,body.getVelocity().y });
	}

	//現在のHP割合を更新
	hp_rate = hp / max_hp;

	hpbar.damage(hpbar.getHP() - hp);//HPの差分処理
	hpbar.update();//hpバーの更新処理

#ifdef _DEBUG
	if (KeyE.pressed() && Key9.pressed()) {//(E + 9)でhpを0にする
		hp = 0;
	}
	if (KeyE.pressed() && Key1.pressed()) {//(E + 1)でhpを10減らす
		getDamage(10);
	}
	if (KeyE.pressed() && Key5.pressed()) {//(E + 5)でビックリマーク生成
		Stage* stage = Stage::GetInstance();
		stage->createObject<ExclamationMarkEffect>(pos, img_flipFlg);
	}
#endif // DEBUG
}

void EnemyBase::draw() const
{
#ifdef _DEBUG
	//画像ができるまでの仮表示
	//Rect(position.x, position.y, size.x, size.y).draw(Palette::Red);
#endif // DEBUG
}

void EnemyBase::onHit(ObjectBase& object)
{
	//プレイヤーに当たったら
	if (Player* player = dynamic_cast<Player*>(&object)) {
		//プレイヤーが攻撃状態じゃないなら
		if (player->getplayerstate() != ePlayerState::attack &&
			player->getplayerstate() != ePlayerState::avoidance &&
			player->getplayerstate() != ePlayerState::jump_attack &&
			player->getplayerstate() != ePlayerState::jump_avoidance &&
			(nowState == ATTACK)) {

			player->applyDamage(10);//プレイヤーへダメージ

			//プレイヤーのノックバック
			if (object.getBody().getPos().x < body.getPos().x)
			{
				object.getBody().applyLinearImpulse(Vec2{ -1, -1 });
			}
			else
			{
				object.getBody().applyLinearImpulse(Vec2{ 1, -1 });
			}
		}
	}
}

void EnemyBase::animation(double delta_second, double idle_ct, double attack_postion_ct, double attack_ct, double get_attack_ct, double die_ct)
{
	switch (nowState)
	{
	case IDLE:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
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
		if (imageChangeTime >= idle_ct && nowImageNum <= idle_img.size()) {
			now_texture = idle_img[nowImageNum];//画像を更新
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	case ATTACK_POSITION:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
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
		if (imageChangeTime >= attack_postion_ct && nowImageNum <= attackPosition_img.size()) {
			now_texture = attackPosition_img[nowImageNum];
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	case ATTACK:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
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
		if (imageChangeTime >= attack_ct && nowImageNum <= attack_img.size()) {
			now_texture = attack_img[nowImageNum];
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	case GET_ATTACK:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
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
		if (imageChangeTime >= get_attack_ct && nowImageNum <= getAttack_img.size()) {
			now_texture = getAttack_img[nowImageNum];
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	case DIE:
		//現在のステートと前フレームのステートが違う場合nowStateTiemを０にする
		if (nowState != oldState) {
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
		if (imageChangeTime >= die_ct && nowImageNum <= die_img.size()) {
			now_texture = die_img[nowImageNum];
			imageChangeTime = 0;
			nowImageNum++;
		}
		break;
	default:
		break;
	}
}

void EnemyBase::movement(float distance, eMovementDirection para)
{
	//X軸方向
	if (para == eMovementDirection::X) {
		if (spawnPosition.x <= body.getPos().x) {
			body.setVelocity(Vec2{ -MOVE_SPEED,body.getVelocity().y });
			img_flipFlg = true;
		}
		if (spawnPosition.x - distance >= body.getPos().x) {
			body.setVelocity(Vec2{ MOVE_SPEED,body.getVelocity().y });
			img_flipFlg = false;
		}
	}
	//Y軸方向
	else if (para == eMovementDirection::Y) {
		if (spawnPosition.y <= body.getPos().y) {
			body.applyLinearImpulse(Vec2{ 0.0,-distance });
		}
	}
	
}

void EnemyBase::drawHP() const
{
	//hpの位置
	Vec2 hp_pos = Vec2{ pos.x,pos.y - (body_size.y - 30) };//ここの値は位置を調整している

	//hpフレーム画像の描画
	hp_frame_img.resized(hp_imgSize.x + 5, hp_imgSize.y + 5).drawAt(hp_pos);
	//hpバー画像の描画
	double rate = 1 - hp_rate;
	hp_bar_img.uv(rate, 1.0, 1.0, 1.0).resized(hp_imgSize.x, hp_imgSize.y).drawAt(hp_pos);
}

void EnemyBase::getDamage(float damage)
{
	//設定した時間経過後またダメージを受ける
	if (damageStopFlg == false) {
		hp -= damage;
		AudioAsset(U"Enemy_damage_SE").playOneShot();
		getDamageFlg = true;
		damageStopFlg = true;

		//ダメージを受けた時のノックバック処理
		if (playerPos.x > body.getPos().x) {
			body.applyLinearImpulse(Vec2{ -130,0 });
			img_flipFlg = false;
		}
		else {
			body.applyLinearImpulse(Vec2{ 130,0 });
			img_flipFlg = true;
		}
	}

	//HPが０以下なら０にする
	if (hp <= 0) {
		hp = 0;
	}
}

void EnemyBase::spawnBuff()
{
	//ステージのインスタンスを取得
	Stage* stage = Stage::GetInstance();

	//タイプによって違う種類のBuffを生成する
	switch (type)
	{
	case scarerun:
		stage->createObject<AttackPowerBuff>(pos);
		break;
	case flot:
		{
			Vec2 spos = pos + Vec2{ 0.0,-15.0 };
			stage->createObject<MoveSpeedBuff>(spos);
			break;
		}
	default:
		break;
	}
	
}

eLookDirection EnemyBase::nowLookDirection()
{
	if (img_flipFlg == true) {
		return eLookDirection::LEFT;
	}
	else if (img_flipFlg == false) {
		return eLookDirection::RIGTH;
	}

	return eLookDirection::LEFT;
}

bool EnemyBase::isPlayerInSight()
{
	//左を向いていてプレイヤーが左にいる場合
	if (eLookDirection::LEFT == nowLookDirection() && 0 > calcPlayerDist().x) {
		//指定した距離内にいる場合
		if (-SIGHT < calcPlayerDist().x /*&& (playerPos.y < 550 && playerPos.y > 400)*/) {
			playerFoundFlg = true;
			return true;
		}
	}//右を向いていてプレイヤーが右にいる場合
	else if (eLookDirection::RIGTH == nowLookDirection() && 0 < calcPlayerDist().x) {
		//指定した距離内にいる場合
		if (SIGHT > calcPlayerDist().x /*&& (playerPos.y < 550 && playerPos.y > 400)*/) {
			playerFoundFlg = true;
			return true;
		}
	}

	return false;
}

void EnemyBase::attackMove()
{
	//プレイヤーまでの距離を取得する
	Vec2 playerDist = calcPlayerDist();
	double length = playerDist.length();

	if (length > 0) {
		body.setVelocity(Vec2{ (playerDist.x / length * (MOVE_SPEED * 10)),body.getVelocity().y });

		if (body.getVelocity().x < 0) {
			img_flipFlg = true;
		}
		else if (body.getVelocity().x > 0) {
			img_flipFlg = false;
		}
	}
}

void EnemyBase::getDamageMove()
{
}

void EnemyBase::doOncePosZero(Vec2 para)
{
	//今のフレームのステートと前のフレームのステートが違う時
	//doOnceFlgを下げる
	if (doOnceFlg == true && nowState != oldState) {
		doOnceFlg = false;
	}

	if (doOnceFlg == false) {
		this->getBody().setVelocity(para);
		doOnceFlg = true;
	}
}

void EnemyBase::spawnAmmo(double speed, bool playerTargetFlg)
{
	//ステージのインスタンスを取得
	Stage* stage = Stage::GetInstance();

	//スポーンする場所を生成する敵の前にする
	Vec2 spawnPos = Vec2{ 0,0 };
	
	if (img_flipFlg == true) {
		spawnPos.x = body.getPos().x - 40;
		spawnPos.y = body.getPos().y;
	}
	else if (img_flipFlg == false) {
		spawnPos.x = body.getPos().x + 40;
		spawnPos.y = body.getPos().y;
	}

	stage->createObject<Ammo>(spawnPos, speed, playerTargetFlg, playerPos);
}
