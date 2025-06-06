#include "Ammo.hpp"
//Player
#include "../Characters/Player.hpp"
//Ground
#include "Ground.hpp"

Ammo::Ammo(P2World& world, const Vec2& position, double setSpeed, bool setPlayerTargetFlg, Vec2 pPos)
	: ObjectBase(world, position)
{
	assetName = U"Ammo";
	margin = Vec2{ 0.0,0.0 };
	lifeTime = 0.0;
	size = Vec2{ 30.0,30.0 };
	playerPos = pPos;

	//物理エンジンでの物体設定（動くか、位置、大きさ）
	body = world.createRect(
		P2Dynamic,
		position,
		size,
		P2Material{ .friction = 0.0 },
		P2Filter{
			.categoryBits = CollisionCategory::Enemy,
			.maskBits = CollisionCategory::All & ~CollisionCategory::Enemy
		}
	);

	speed = setSpeed;//発射速度
	playerTargetFlg = setPlayerTargetFlg;//プレイヤーに向かって発射するか

	//
	if (playerPos.x < body.getPos().x) {
		img_flipFlg = true;
	}
	else if(playerPos.x > body.getPos().x){
		img_flipFlg = false;
	}
}

Ammo::~Ammo()
{
}

void Ammo::update()
{
	//現在の生存時間を更新
	lifeTime += Scene::DeltaTime();
	//移動
	movement();
	//画像の回転
	img_rotated = std::atan2(body.getVelocity().y, body.getVelocity().x);
	if (img_flipFlg) {//画像を左右反転
		img_rotated += Math::TwoPi;
	}

	//アニメーション
	animation();


	if (body) {
		pos = body.getPos();
	}

	if (lifeTime >= _LIFE_TIME_) {
		deleteSelf();
	}
}

void Ammo::draw() const
{

	TextureAsset(assetName)(margin, Vec2{ _IMG_SIZE_X_,_IMG_SIZE_Y_ })
		.resized(Vec2{ _IMG_RESIZE_X_,_IMG_RESIZE_Y_ })
		.rotated(img_rotated)
		.drawAt(pos);
#ifdef _DEBUG
	//当たり判定表示
	body.drawFrame();
#endif // DEBUG
}

void Ammo::onHit(ObjectBase& object)
{
	//プレイヤーに当たった時
	if (Player* p = dynamic_cast<Player*>(&object)) {
		p->onDamaged(10);
		deleteSelf();
	}
	//地面に当たった時
	if (Ground* g = dynamic_cast<Ground*>(&object)) {
		deleteSelf();
	}
}

void Ammo::movement()
{
	//プレイヤーの方向へ移動
	if (playerTargetFlg == true) {
		if (!initialized) {
			Vec2 playerDist = playerPos - body.getPos();
			double length = playerDist.length();

			if (length > 0) {
				moveDirection = playerDist / length;
				initialized = true;
			}
		}

		if (initialized) {
			body.setVelocity(moveDirection * speed);

			if (body.getVelocity().x < 0) {
				img_flipFlg = true;
			}
			else if (body.getVelocity().x > 0) {
				img_flipFlg = false;
			}
		}
	}
	else {

	}
}

void Ammo::animation()
{
	//画像表示時間を更新
	nowImgTime += Scene::DeltaTime();

	if (nowImgTime >= _IMG_CHANG_TIME_) {
		nowImgTime = 0;
		margin = margin + Vec2{ _IMG_SIZE_X_,0.0 };

		if (margin.x == 1152.0) {
			margin = Vec2{ 0.0,0.0 };
		}
	}
}
