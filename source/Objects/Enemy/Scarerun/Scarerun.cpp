#include "Scarerun.hpp"
//画像の分割読み込み
#include "../../../Utils/CustomImageLoader.hpp"

Scarerun::Scarerun(const Vec2& start_position):
	EnemyBase(start_position)//初期位置
{
	//画像を分割読み込み
	idle_img = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//idle画像
	now_texture = idle_img[0];//初期化用の画像


	size = Vec2(100, 100);
}

Scarerun::~Scarerun()
{
}

void Scarerun::update()
{
	__super::update();

	if (nowState == IDLE) {
		movement(30.0f);//左右移動（数値は移動する距離）
	}
	if (hp <= 0) {
		setEnemyState(DIE);
	}
}

void Scarerun::draw() const
{
	now_texture.mirrored(img_flipFlg).resized(size).drawAt(position);
}
