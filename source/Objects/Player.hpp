# pragma once

#include "CharacterBase.hpp"
#include <Siv3D/XInput.hpp>

enum ePlayerState
{
	idle,		//待機
	move,		//移動
	jump,		//ジャンプ
	avoidance,	// 回避
	attack,		//攻撃
	jump_attack,//ジャンプ攻撃
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

public:

	Player(const Vec2& start_position);
	~Player();

	void initialize();
	void update() override;
	void draw() const override;
	void finalize();

	ePlayerState getplayerstate() const;
	//ePlayerState setplayerstate(int state);	//デバッグの時に使うかも

private:
	void animation();	//アニメーション
	void movement(s3d::detail::XInput_impl controller);	//左右移動
	//void jumpmovement(s3d::detail::XInput_impl controller);//ジャンプ処理
};
