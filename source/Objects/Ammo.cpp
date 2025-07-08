#include "Ammo.hpp"
//Player
#include "../Characters/Player.hpp"
//Ground
#include "Ground.hpp"

Ammo::Ammo(P2World& world, const Vec2& position, double setSpeed, bool setPlayerTargetFlg, Vec2 pPos)
	: ObjectBase(world, position)
{
	assetName = U"Ammo";//使用する画像
	margin = Vec2{ 0.0,0.0 };//使用画像の最初に使う左上の位置
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

	//画像を反転するか判定する
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
	
	if (body) {
		pos = body.getPos();

		img_rotated = std::atan2(body.getVelocity().y, body.getVelocity().x);
		if (img_flipFlg) {//画像を左右反転
			img_rotated += Math::TwoPi;
		}
		body.setAngle(img_rotated);
	}

	//アニメーション
	animation();

	//指定した時間になったら自分をデストロイ
	if (lifeTime >= _LIFE_TIME_) {
		destroy();
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

void Ammo::onHit(ObjectBase& object, const P2Collision& collision)
{
	//プレイヤーに当たった時
	if (Player* p = dynamic_cast<Player*>(&object)) {
		if (p->getplayerstate() != ePlayerState::attack &&
			p->getplayerstate() != ePlayerState::avoidance &&
			p->getplayerstate() != ePlayerState::jump_attack &&
			p->getplayerstate() != ePlayerState::jump_avoidance)
		{
			p->onDamaged(10);
		}
		destroy();
	}
	//地面に当たった時
	if (Ground* g = dynamic_cast<Ground*>(&object)) {
		destroy();
	}
}

void Ammo::destroy()
{
	body.release();//当たり判定を消す
	if (!destroyInitFlg) {//初期化処理（一度のみ）
		margin = Vec2{ 0.0,0.0 };
		assetName = U"Ammo Impact";

		destroyInitFlg = true;
	}
	

	if (destroyAnimEndFlg) {
		ObjectBase::destroy();
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

	if (nowImgTime >= _IMG_CHANG_TIME_) {//画像切替
		nowImgTime = 0;

		if (assetName == U"Ammo") {//通常画像
			margin = margin + Vec2{ _IMG_SIZE_X_,0.0 };

			if (margin.x == 1152.0) {
				margin = Vec2{ 0.0,0.0 };
			}
		}
		else if (assetName == U"Ammo Impact") {//消滅時の画像
			margin = margin + Vec2{ 0.0,_IMG_SIZE_Y_ };

			if (margin.y == 1024) {
				destroyAnimEndFlg = true;
			}
		}
	}
}
