#pragma once

//親クラス
#include "../../../Objects/ObjectBase.hpp"

#define IMG_SIZE 64  //縦横64サイズの画像
#define IMG_RESIZE 90//サイズ変更時の値

#define IMG_CHANGE_TIME 0.05//画像切替時間

//色ごとのmargin位置
#define BUFF_EFFECT_COLOR_WHITE  320 //白
#define BUFF_EFFECT_COLOR_RED    0   //赤
#define BUFF_EFFECT_COLOR_PURPLE 64  //紫
#define BUFF_EFFECT_COLOR_BLUE   128 //青
#define BUFF_EFFECT_COLOR_GREEN  192 //緑
#define BUFF_EFFECT_COLOR_ORANGE 256 //オレンジ

class BuffSpawnEffect : public ObjectBase
{
public:
	BuffSpawnEffect(P2World& world, const Vec2& postion, ObjectBase& ownerClass, String color);
	~BuffSpawnEffect();

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

private:
	//エフェクトの色設定
	Vec2 setEffectColor(String color);
};

