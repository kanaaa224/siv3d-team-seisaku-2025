#pragma once
//親クラス
#include "../CharacterBase.hpp"
//Buffの親クラス
#include "../Buff/BuffBase.hpp"

#define DEBUG                  //デバック表示
#define IMG_CHANGE_TIME 0.1f   //画像の切り替え速度
#define MOVE_SPEED      20.0f  //移動速度
#define GRAVITY 9.807f        //重力

#define HP_X_MAXSIZE 50       //HPバーX軸の最大サイズ
#define HP_Y_SIZE 10          //HPバーY軸の大きさ

#define DAMAGE_STOP_TIME 1.0f //ダメージストップ時間

#define SPAWN_BUFF_TIME 1.0f  //ステータスがDIEになってから何秒でBuffを生成するか

#define SIGHT 200             //視力

enum eEnemyType
{
	Scarerun
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

class EnemyBase : public CharacterBase
{
protected:
	eEnemyType type;                    //敵のタイプ

	eEnemyState nowState;               //現在の状態
	eEnemyState oldState;               //1フレーム前の状態
	float nowStateTime;                 //現在の状態へ遷移してからの経過時間

	Vec2 playerPos;                     //プレイヤーの現在地
	bool playerFoundFlg;                //プレイヤーを発見したか

	TextureRegion now_texture;              //現在の画像

	Array<TextureRegion> idle_img;          //移動時の画像
	Array<TextureRegion> attackPosition_img;//攻撃姿勢の画像
	Array<TextureRegion> attack_img;        //攻撃の画像
	Array<TextureRegion> getAttack_img;     //攻撃を受ける画像
	Array<TextureRegion> die_img;           //死亡の画像
	int nowImageNum;                        //画像配列の現在の要素数を格納する用
	float imageChangeTime;                  //画像切替用変数

	bool img_flipFlg;                       //画像を左右反転

	Vec2 spawnPosition;                 //スポーン位置を格納

	float damageStop_ct;                //ダメージを受けてから何秒経過しているか
	bool damageStopFlg;                 //ダメージを受けるのを止めるか止めないか

	P2World* nowWorld;                  //現在の物理シュミレーション

	bool spawnBuffFlg;                  //buffが生成されたか

	bool getDamageFlg;                  //ダメージを受けた時にTrue、ダメージを受けたアニメーションが終わったらFalse

	bool doOnceFlg;

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
	virtual void animation(float delta_second);
	/// <summary>
	/// 敵のステートをセット
	/// </summary>
	/// <param name="setState"></param>
	virtual void setEnemyState(eEnemyState setState) {
		if (oldState != setState) {
			oldState = nowState;
			nowState = setState;

			nowStateTime = 0.0f;
		}
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

	//状態遷移
	virtual void stateControl() {};

	//バフをスポーンさせる
	void spawnBuff();

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
public:
	void setPlayerPos(Vec2 pos) { playerPos = pos; };
};

