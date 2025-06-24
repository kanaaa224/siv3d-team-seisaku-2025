#pragma once

//親クラス
#include "../../../Objects/Base.hpp"

#define IMG_SIZE_X 532 //横532サイズの画像
#define IMG_SIZE_Y 528 //縦528サイズの画像
#define IMG_RESIZE 90//サイズ変更時の値

#define IMG_CHANGE_TIME 0.005//画像切替時間

class SmallHitEffect : public ObjectBase
{
public:
	SmallHitEffect(P2World& world, const Vec2& postion, ObjectBase& ownerClass);
	~SmallHitEffect();

	void update();
	void draw() const;

private:
	String assetName;    //使用する画像のタグ
	Vec2 margin;         //画像の左上の位置座標
	Vec2 reSize;         //画像のサイズ変更
	bool img_flipFlg;    //画像を反転するか

	Vec2 pos;            //描画する座標

	double lifeTime;     //現在の生存時間
	double imgChange_ct;//画像切替の計測用

	bool animationEndFlg;//画像が最後の分割領域にきたか

	ObjectBase* owner;
};

