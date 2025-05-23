﻿#pragma once
//親クラス
#include "../CharacterBase.hpp"
//Buffの親クラス
#include "../Buff/BuffBase.hpp"

#define DEBUG                  //デバック表示
#define IMG_CHANGE_TIME 0.1f  //画像の切り替え速度
#define MOVE_SPEED      20.0f   //移動速度
#define FALLING_SPEED   4.0f   //落下速度
#define GRAVITY 9.807f //重力

#define HP_X_MAXSIZE 50       //HPバーX軸の最大サイズ
#define HP_Y_SIZE 10          //HPバーY軸の大きさ

#define DAMAGE_STOP_TIME 0.1f //ダメージストップ時間

enum eEnemyState
{
	NONE,           //何もなし
	IDLE,           //待機状態
	ATTACK_POSITION,//攻撃姿勢状態
	ATTACK,         //攻撃状態
	GET_ATTACK,     //攻撃を受ける状態
	DIE             //死亡状態
};

class EnemyBase : public CharacterBase
{
protected:
	eEnemyState nowState;               //現在の状態
	eEnemyState oldState;               //1フレーム前の状態
	float nowStateTime;                 //現在の状態へ遷移してからの経過時間

	Vec2 playerLocation;                //プレイヤーの現在地
	Vec2 playerVelocity;                //プレイヤーの現在の移動量
	bool playerFoundFlg;                //プレイヤーを発見したか

	TextureRegion now_texture;              //現在の画像

	Array<TextureRegion> idle_img;          //移動時の画像
	Array<TextureRegion> attackPosition_img;//攻撃姿勢の画像
	Array<TextureRegion> attack_img;        //攻撃の画像
	Array<TextureRegion> getAttack_img;     //攻撃を受ける画像
	Array<TextureRegion> die_img;           //死亡の画像
	int nowImageNum;                        //画像配列の現在の要素数を格納する用
	float imageChangeTime;                  //画像切替用変数

	bool img_flipFlg;

	Vec2 spawnPosition;                 //スポーン位置を格納

	float damageStop_ct;                //ダメージを受けてから何秒経過しているか
	bool damageStopFlg;                 //ダメージを受けるのを止めるか止めないか

	P2World* nowWorld;                   //現在の物理シュミレーション

	bool spawnBuffFlg;

public:
	EnemyBase(P2World& world, const Vec2& position);
	~EnemyBase();

	virtual void update() override;
	virtual void draw() const override;

protected:
	/// <summary>
	/// ステートに合わせて画像を切り替える
	/// </summary>
	/// <param name="delta_second">1フレーム何秒経過したか</param>
	virtual void animation(float delta_second);
	/// <summary>
	/// 敵のステートをセット
	/// </summary>
	/// <param name="setState"></param>
	virtual void setEnemyState(eEnemyState setState) {
		oldState = nowState;
		nowState = setState;
	};
	/// <summary>
	/// 基本的な左右移動
	/// ※飛行する敵などは書き換える
	/// スポーン位置を始点に引数に入れたdistance分移動したら、
	/// 始点に方向転換して移動する
	/// </summary>
	/// <param name="distance"></param>
	virtual void movement(float distance);

	/// <summary>
	/// ダメージを受けた時の移動
	/// </summary>
	virtual void getDamageMovement() {};

	/// <summary>
	/// デバッグ用のHP表示
	/// </summary>
	void drawHP() const;

	/// <summary>
	/// ダメージを受けた時にダメージ数を入れる関数
	/// ダメージストップも担う
	/// </summary>
	/// <param name="damage">ダメージ数</param>
	void getDamage(float damage);

	//バフをスポーンさせる
	void spawnBuff();
};

