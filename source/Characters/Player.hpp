# pragma once

# include "Base.hpp"
# include "../Effects/SpriteAnimator.hpp"

enum ePlayerState
{
	idle,			//待機
	move,			//移動
	jump,			//ジャンプ
	avoidance,		//回避
	attack,			//攻撃
	jump_attack,	//空中攻撃
	jump_avoidance,	//空中回避
	damage,			//ダメージ
	die,			//死亡
	null,			//何もなし(初期化用)
};

enum eItemState
{
	//バフアイテムの追加
};

class Player : public CharacterBase
{
private:
	ePlayerState playerState;	//状態管理
	
	bool is_on_ground;			//地面についているか？

	size_t playerIndex;			//プレイヤー数
	bool enableDeadZone;		//デッドゾーンを有効にするか？

	bool flip_flg;				//画像反転フラグ
	bool jump_attack_flg;

	TextureRegion image;		//画像描画用

	Array <TextureRegion> idle_animation;			//待機状態アニメーション
	Array <TextureRegion> run_animation;			//移動状態アニメーション

	Array <TextureRegion> jump_up_animation;		//ジャンプ開始状態アニメーション
	Array <TextureRegion> jump_full_animation;		//ジャンプ中間状態アニメーション
	Array <TextureRegion> jump_down_animation;		//ジャンプ降下状態アニメーション

	Array <TextureRegion> attack_animation;			//攻撃状態アニメーション
	Array <TextureRegion> jump_attack_animation;    //ジャンプ攻撃状態アニメーション

	Array <TextureRegion> jump_attack_2_animation;	//ジャンプ攻撃

	Array <TextureRegion> roll_animation;			//回避状態アニメーション
	Array <TextureRegion> damage_animation;			//ダメージアニメーション
	Array <TextureRegion> die_animation;			//死亡アニメーション

	int8 animation_number;		//画像切り替え用

	double animation_time;      //アニメーション時間

	float ground_y;

	bool isTriggeredOnce;		//一度だけの処理用フラグ
	bool wall_hit;

	P2Filter filter;			//物理干渉のフィルター

	double hitStopTimer;  //ヒットストップタイマー
	bool isHitStop;     //ヒットストップしたかどうか
	bool Damageflg;		//ダメージフラグ

	double avoidanceCooldown; // 回避のクールタイム秒数
	double avoidanceCooldownDuration; // クールタイム間隔

	double alpha;
	double movement_speed;
	double attack_power;

	bool enemyHit;

	SpriteAnimator spriteAnimator;

	Size effect_size;

	Vec2 pos;

	s3d::Audio m_runLoopSE; // 走行ループSE用

public:

	Player(P2World& world, const Vec2& position);
	~Player();

	void initialize();
	void update() override;
	void draw() const override;
	void finalize();

	ePlayerState getplayerstate() const;
	//void setplayerstate(ePlayerState state);	//デバッグの時に使うかも

	void onHit(ObjectBase& object) override;
	void onDamaged(float amount) override;	//ダメージを受けた時の処理
	void destroy() override;				//死体が消える系

	void die() override;					//死亡処理

	bool getFlip()    const { return flip_flg; }

	// バフステータス
	void SetPlayerSpeed(double speed) { movement_speed += speed; };
	void SetPlayerDamageUp(double damage_up) { attack_power += damage_up; };
	double GetPlayerAttackDamage() { return attack_power; };

	// 回避クールタイムの残り秒数を取得
	double getAvoidanceCooldown() const { return avoidanceCooldown; }

	//
	void SetHitEnemy(bool _flg) { enemyHit = _flg; };

	//eItemState getItemstate() const;   //itemのステータスを返す
	//setItemstate(eItemState item);  //itemの設定

	s3d::Audio run_se;

private:
	void animation(Array<TextureRegion> image_container, double frame,int image_count, ePlayerState state);	//アニメーション
	bool animation(Array<TextureRegion> image_container, double frame);	//攻撃アニメーション用関数
	void movement(s3d::detail::XInput_impl controller);	//左右移動
	void jumpmovement(s3d::detail::XInput_impl controller);//ジャンプ処理
};
