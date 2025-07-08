#pragma once

//親クラス
#include "ObjectBase.hpp"

#define _LIFE_TIME_ 3.0//生成されて[_LIFE_TIME_]秒後削除

#define _IMG_CHANG_TIME_ 0.1//画像切替速度
#define _IMG_SIZE_X_ 128
#define _IMG_SIZE_Y_ 128
#define _IMG_RESIZE_X_ 70
#define _IMG_RESIZE_Y_ 70

class Ammo : public ObjectBase
{
private:
	double lifeTime;

	Vec2 size;
	Vec2 playerPos;
	Vec2 pos;

	double speed;        //玉の発射速度
	bool playerTargetFlg;//プレイヤーに向かって玉が飛ぶか

	Vec2 moveDirection;
	bool initialized = false;

	//画像関係
	String assetName;   //画像のタグ
	Vec2 margin;        //画像の左上
	double img_rotated; //画像の角度
	bool img_flipFlg;   //画像を左右反転
	double nowImgTime;  //現在の画像の表示時間
	bool destroyAnimEndFlg;//玉が消えるアニメーションが終わったか

	bool destroyInitFlg;//destroy関数内で初期化をやったか

public:
	Ammo(P2World& world, const Vec2& position, double setSpeed, bool setPlayerTargetFlg, Vec2 pPos);
	~Ammo() override;

	void update() override;
	void draw() const override;

	void onHit(ObjectBase& object, const P2Collision& collision) override;

	void destroy() override;

private:
	//移動処理
	void movement();
	//アニメーション
	void animation();
};

