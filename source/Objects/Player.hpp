# pragma once

# include "CharacterBase.hpp"

enum ePlayerState
{
	idle,		//待機
	move,		//移動
	jump,		//ジャンプ
	avoidance,	//回避
	attack,		//攻撃
	jump_attack,//ジャンプ攻撃
	damage,     //ダメージ
	die,		//死亡
	null,		//何もなし(初期化用)
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

	Array <TextureRegion> idle_animation;	//待機状態アニメーション
	Array <TextureRegion> run_animation;    //移動状態アニメーション
	Array <TextureRegion> attack_animation;    //攻撃状態アニメーション
	Array <TextureRegion> roll_animation;    //回避状態アニメーション

	int8 animation_number;		//画像切り替え用

	double animation_time;      //アニメーション時間

	float ground_y;

	//プレイヤーの状態
	int player_s;
	//

public:

	Player(P2World& world, const Vec2& position);
	~Player();

	void initialize();
	void update() override;
	void draw() const override;
	void finalize();

	ePlayerState getplayerstate() const;
	//ePlayerState setplayerstate(int state);	//デバッグの時に使うかも



private:
	void animation(Array<TextureRegion> image_container, double frame,int image_count, ePlayerState state);	//アニメーション
	void movement(s3d::detail::XInput_impl controller);	//左右移動
	void jumpmovement(s3d::detail::XInput_impl controller);//ジャンプ処理
};
