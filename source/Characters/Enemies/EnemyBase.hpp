#pragma once
//親クラス
#include "../CharacterBase.hpp"
//Buffの親クラス
#include "../../Objects/Buff/BuffBase.hpp"
//hpBar
#include "../../UI/HPBar.hpp"

#define MOVE_SPEED 20.0f      //移動速度

#define HP_X_MAXSIZE 50       //HPバーX軸の最大サイズ
#define HP_Y_SIZE 10          //HPバーY軸の大きさ

#define DAMAGE_STOP_TIME 0.65f //ダメージストップ時間

#define SPAWN_BUFF_TIME 1.0f  //ステータスがDIEになってから何秒でBuffを生成するか

#define SIGHT 200             //視力

#define KNOCKBACK 90         //自分のノックバックの値

enum eEnemyType
{
	scarerun,
	flot
};

enum eEnemyState
{
	NONE,           //何もなし
	IDLE,           //待機状態
	ATTACK_POSITION,//攻撃姿勢状態
	ATTACK,         //攻撃状態
	GET_ATTACK,     //攻撃を受ける状態
	DIE             //死亡状態
};

enum eLookDirection
{
	RIGTH,
	LEFT
};

enum eMovementDirection
{
	X,
	Y
};

class EnemyBase : public CharacterBase
{
protected:
	HPBar hpbar;                            //HPバー

	eEnemyType type;                        //敵のタイプ
	Vec2 body_size;                         //bodyの大きさ

	eEnemyState nowState;                   //現在の状態
	eEnemyState oldState;                   //1フレーム前の状態
	double nowStateTime;                    //現在の状態へ遷移してからの経過時間

	Vec2 playerPos;                         //プレイヤーの現在地
	bool playerFoundFlg;                    //プレイヤーを発見したか

	TextureRegion now_texture;              //現在の画像

	Array<TextureRegion> idle_img;          //移動時の画像
	Array<TextureRegion> attackPosition_img;//攻撃姿勢の画像
	Array<TextureRegion> attack_img;        //攻撃の画像
	Array<TextureRegion> getAttack_img;     //攻撃を受ける画像
	Array<TextureRegion> die_img;           //死亡の画像
	int nowImageNum;                        //画像配列の現在の要素数を格納する用
	double imageChangeTime;                 //画像切替用変数
	Vec2 img_size;                          //画像サイズ
	Vec2 drawExcMark_pos;                   //ビックリエフェクトを出す場所

	bool img_flipFlg;                       //画像を左右反転

	TextureRegion hp_frame_img;             //hpバーの後ろ画像
	const Texture hp_bar_img{ TextureAsset(U"Enemy HP Bar") };//hpバーの前画像
	Vec2 hp_imgSize;                        //hpバーの画像サイズ
	double hp_rate;

	Vec2 spawnPosition;                     //スポーン位置を格納

	double damageStopTime;                  //ダメージストップ時間
	double damageStop_ct;                   //ダメージを受けてから何秒経過しているか
	bool damageStopFlg;                     //ダメージを受けるのを止めるか止めないか

	P2World* nowWorld;                      //現在の物理シュミレーション

	bool spawnBuffFlg;                      //buffが生成されたか

	bool getDamageFlg;                      //ダメージを受けた時にTrue、ダメージを受けたアニメーションが終わったらFalse

	bool doOnceFlg;

	Vec2 pos;

	bool doOnceDieSE;

	//waitMovement関数で使う変数
	double waitMoveTimer;
	bool waitMoveFlg;

public:
	EnemyBase(P2World& world, const Vec2& position);
	~EnemyBase();

	virtual void update() override;
	virtual void draw() const override;

	virtual void onHit(ObjectBase& object) override;

	virtual void onDamaged(float amount) override { getDamage(amount); };

protected:
	/// <summary>
	/// ステートに合わせて画像を切り替える
	/// </summary>
	/// <param name="delta_second">1フレーム何秒経過したか</param>
	virtual void animation(double delta_second, double idle_ct, double attack_postion_ct, double attack_ct, double get_attack_ct, double die_ct);
	/// <summary>
	/// 敵のステートをセット
	/// </summary>
	/// <param name="setState"></param>
	virtual void setEnemyState(eEnemyState setState) {
		if (oldState != setState) {
			oldState = nowState;
			nowState = setState;

			nowStateTime = 0.0f;
			nowImageNum = 0;
		}
	};
	/// <summary>
	/// 基本的な左右移動
	/// ※飛行する敵などは書き換える
	/// スポーン位置を始点に引数に入れたdistance分移動したら、
	/// 始点に方向転換して移動する
	/// </summary>
	/// <param name="distance"></param>
	virtual void movement(float distance, eMovementDirection para);

	/// <summary>
	/// ダメージを受けた時の移動
	/// </summary>
	virtual void getDamageMovement() {};

	//hpバーの表示
	void drawHP() const;

	/// <summary>
	/// ダメージを受けた時にダメージ数を入れる関数
	/// ダメージストップも担う
	/// </summary>
	/// <param name="damage">ダメージ数</param>
	void getDamage(float damage);

	//状態遷移
	virtual void stateControl() {};

	//プレイヤーまでの距離
	Vec2 calcPlayerDist() { return playerPos - body.getPos(); };

	//敵が左右どちらを向いているか
	eLookDirection nowLookDirection();

	//プレイヤーを視野内に捕らえているか
	bool isPlayerInSight();

	//攻撃時の移動
	void attackMove();

	//攻撃を受けた時の移動
	void getDamageMove();

	//１種類のステータスの処理内で１回のみ座標にゼロを入れる
	void doOncePosZero(Vec2 para);

	// スポーン処理 //
	//玉の生成
    void spawnAmmo(double speed,bool playerTargetFlg);
	//ビックリマークの生成
	void spawnExclamationMarkEffect(ObjectBase& obj);
	bool doOnecSpawnExclamationMarkFlg;
	//バフをスポーンさせる
	void spawnBuff();

	//足を止める処理
	void waitMovement(double waitTime);
public:
	void setPlayerPos(Vec2 pos) { playerPos = pos; };

	Vec2 getDrawExcMarkPos() { return drawExcMark_pos; };
	Vec2 getPosition() { return pos; };
};

