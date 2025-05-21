#include "Scarerun.hpp"
//画像の分割読み込み
#include "../../../Utils/CustomImageLoader.hpp"

Scarerun::Scarerun(P2World& world, const Vec2& position):
	EnemyBase(world, position)//初期位置
{
	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(P2Dynamic, position, SizeF{ 50, 60 },P2Material{ .friction = 0.0 });
	body.setFixedRotation(true);//当たり判定の回転を無くす

	//画像を分割読み込み
	idle_img = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//idle画像
	die_img = LoadDivGraph(U"Scarerun Idle", Size(46, 40));//die仮画像
	now_texture = idle_img[0];//初期化用の画像


	//size = Vec2(100, 100);
}

Scarerun::~Scarerun()
{
}

void Scarerun::update()
{
	__super::update();

	if (nowState == IDLE) {
		movement(70.0f);//左右移動（数値は移動する距離）
	}
	if (hp <= 0) {
		setEnemyState(DIE);
		body.setVelocity(Vec2{ 0.0,body.getVelocity().y });
	}
}

void Scarerun::draw() const
{
	Vec2 size = Vec2(100, 100);

	now_texture.mirrored(img_flipFlg).resized(size).drawAt(body.getPos());
	body.drawFrame();

	Print << U"Enemy_Velocity : " << body.getVelocity();
	Print << U"Enemy_SpawnPos : " << spawnPosition;
	Print << U"Enemy_Pos : " << body.getPos();
	Print << U"NowState : " << nowState;
	Print << U"OldState : " << oldState;
	Print << U"StateNum : " << nowStateTime;
	Print << U"HP : " << hp;
}
