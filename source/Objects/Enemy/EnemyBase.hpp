#pragma once
//親クラス
#include "../CharacterBase.hpp"

#define DEBUG                  //デバック表示
#define IMG_CHANGE_TIME 0.05f  //画像の切り替え速度
#define MOVE_SPEED      1.0f   //移動速度
#define FALLING_SPEED   4.0f   //落下速度

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
private:
	eEnemyState nowState;               //現在の状態
	eEnemyState oldState;               //1フレーム前の状態
	float nowStateTime;                 //現在の状態へ遷移してからの経過時間

	Vec2 playerLocation;                //プレイヤーの現在地
	Vec2 playerVelocity;                //プレイヤーの現在の移動量
	bool playerFoundFlg;                //プレイヤーを発見したか

	std::vector<Texture> idle_img;          //移動時の画像
	std::vector<Texture> attackPosition_img;//攻撃姿勢の画像
	std::vector<Texture> attack_img;        //攻撃の画像
	std::vector<Texture> getAttack_img;     //攻撃を受ける画像
	std::vector<Texture> die_img;           //死亡の画像
	int nowImageNum;                        //画像配列の現在の要素数を格納する用

	Vec2 spawnPosition;                 //スポーン位置を格納

	bool initUpdateFlg;                 //Update内で初期化したか

	float damageStop_ct;                //ダメージを受けてから何秒経過しているか
	bool damageStopFlg;                 //ダメージを受けるのを止めるか止めないか

public:
	EnemyBase(const Vec2& start_position);
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
	/// Update内で１回のみ初期化したい変数を関数内に入れる
	/// </summary>
	virtual void initUpdate();

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
};

