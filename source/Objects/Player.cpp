# include "Player.hpp"
# include "../Utils/CustomImageLoader.hpp"

#define VELOCITY 150.0	   //移動速度
#define D_GRAVITY (9.807f) //重力
#define ADDJUMP (10)	   //

#define DEBUG

Player::Player(P2World& world, const Vec2& position) : CharacterBase(world, position), M_world(world)
{
	body = world.createRect(P2Dynamic, position, SizeF{ 100, 100 - 10 }); // 島袋が追記: 物理シミュレーションを行うための箱を生成

	is_on_ground = false;
	playerState = ePlayerState::null;
	playerIndex = NULL;
	enableDeadZone = false;
	flip_flg = false;
	animation_number = 0;

	this->initialize();
}

Player::~Player()
{
	this->finalize();

	idle_animation.clear();
}

void Player::initialize()
{
	//size = Vec2(288.0 * 2, 45.0 * 2);	//サイズ設定
	is_on_ground = true;		//地面についているか？
	playerState = ePlayerState::null;	//待機状態に設定
	playerIndex = 0;			//プレイヤーコントローラー 0番目
	enableDeadZone = false;		//デッドゾーン無効化
	//velocity = Vec2(0.0, 0.0);	//移動量初期化
	flip_flg = false;
	hp = 100;
	ground_y = 640.0f;  //地面のlocation

	// 分割画像の登録
	idle_animation = LoadDivGraph(U"Player Idle", Size(288, 45));
	run_animation = LoadDivGraph(U"Player Run", Size(288, 45));
	attack_animation = LoadDivGraph(U"Player Attack", Size(288, 45));
	roll_animation = LoadDivGraph(U"Player Roll", Size(288, 45));

	image = idle_animation[0];

	player_s = 0;

	body.setVelocity(Vec2(0.0, 0.0));
}

void Player::update()
{
	// 指定したプレイヤーインデックスの XInput コントローラを取得
	auto controller = XInput(playerIndex);

	switch (playerState)
	{
	case null: //何もなし

		// デッドゾーン
		if (!enableDeadZone)
		{
			// デッドゾーンを無効化
			controller.setLeftTriggerDeadZone(DeadZone{});
			controller.setRightTriggerDeadZone(DeadZone{});
			controller.setLeftThumbDeadZone(DeadZone{});
			controller.setRightThumbDeadZone(DeadZone{});
		}

		playerState = ePlayerState::idle;

	case idle: //待機処理

		player_s = 0;

		//移動量なし
		body.setVelocity(Vec2(body.getVelocity()));

		animation(idle_animation, 0.1,8,idle);

		//idle状態からボタンを押したごとの処理
		if (//move
			controller.buttonLeft.pressed() == true ||
			controller.buttonRight.pressed() == true ||
			KeyA.pressed() == true ||
			KeyD.pressed() == true ||
			KeyLeft.pressed() == true ||
			KeyRight.pressed() == true
			)
		{
			playerState = ePlayerState::move;
		}//jump
		else if (controller.buttonA.down() == true && is_on_ground == true || KeySpace.down() == true && is_on_ground == true)
		{
			playerState = ePlayerState::jump;
		}//attack
		else if (controller.buttonX.down() == true || KeyE.down() == true)
		{
			playerState = ePlayerState::attack;
		}//avoidance
		else if (controller.buttonB.down() == true || KeyQ.down() == true)
		{
			playerState = ePlayerState::avoidance;
		}///////////////キーを押したらダメージを減らす
		else if (KeyS.down() == true)
		{
			playerState = ePlayerState::damage;
		}///////////////


		break;
	case move: //移動処理

		player_s = 0;
		movement(controller);
		animation(run_animation, 0.1,8,idle);

		if (controller.buttonA.pressed() == true && is_on_ground == true || KeySpace.pressed() == true && is_on_ground == true)
		{
			playerState = ePlayerState::jump;
		}


		break;
	case jump: //ジャンプ処理
		player_s = 1;
		jumpmovement(controller);

		//地面についた時の処理
		if (body.getVelocity().y == 0.0) {

			is_on_ground = true;
			//jump_attack_flg = false;

			playerState = ePlayerState::idle;
		}

		break;
	case avoidance: //回避処理
		player_s = 2;
		animation(roll_animation, 0.1,7,idle);

		break;
	case attack: //攻撃処理
		player_s = 3;
		animation(attack_animation, 0.1,6,idle);

		//playerState = ePlayerState::idle;

		break;
	case jump_attack: //ジャンプ攻撃処理

		break;
	case die: //死亡処理

		break;
	default: //例外
		break;
	}

	// 画面左側を超えないようにする
	if (body.getPos().x < 55.0)
	{
		body.setPos(55.0, body.getPos().y);
	}

	// 移動量計算
	body.setPos(body.getPos() + body.getVelocity() *  Scene::DeltaTime());
}

void Player::draw() const
{
	//.drawAtを使って中心座標を元に描画
	//TextureAsset(U"Player_idle").mirrored(flip_flg).resized(size).drawAt(position);

	Vec2 size = Vec2(288.0 * 2, 45.0 * 2);

	body.drawFrame(1.0, ColorF(Palette::Blue));
	image.mirrored(flip_flg).resized(size).drawAt(body.getPos());

#ifdef DEBUG
	Print << U"Player 画像登録 : " << TextureAsset::IsReady(U"Player Idle");
	Print << U"Player 座標 : " << body.getPos();
	Print << U"Player 移動量 : " << body.getVelocity();
	Print << U"DeltaTime : " << Scene::DeltaTime();
	Print << U"PlayerState : " << player_s;

	for (auto&& [pair, collision] : M_world.getCollisions())
	{
		// body.id は Playerのこと
		if (pair.a == body.id())
		{
			Print << U"PlayerHit : " << pair.a;
		}
		else if(pair.b == body.id())
		{
			Print << U"PlayerHit : " << pair.b;
		}
		
	}
#endif // DEBUG
}

void Player::onHit(CharacterBase& object)
{
	object.getBody().applyLinearImpulse(Vec2{ 0, -10 });
}

void Player::finalize()
{

}

ePlayerState Player::getplayerstate() const
{
	return playerState;
}

void Player::animation(Array<TextureRegion> image_container, double frame,int image_count, ePlayerState state)
{
	animation_time += Scene::DeltaTime();

	if (animation_time >= frame)
	{
		animation_time = 0.0;
		animation_number++;
		if (animation_number >= image_container.size())
		{
			animation_number = 0;
			if (state == jump && jump_attack_flg == false)
			{
				jump_attack_flg = true;
			}
			playerState = state;
		}

		image = image_container[animation_number];
	}
}

void Player::movement(s3d::detail::XInput_impl controller)
{
	//移動処理
	if (
		controller.buttonLeft.pressed() == true ||
		KeyA.pressed() == true ||
		KeyLeft.pressed() == true
		)
	{

		body.setVelocity(Vec2(-VELOCITY, 0.0));

		flip_flg = true;

		//移動状態のときボタンを押されたら

	}
	else if(controller.buttonRight.pressed() == true || KeyD.pressed() == true || KeyRight.pressed() == true)
	{

		body.setVelocity(Vec2(VELOCITY, 0.0));

		flip_flg = false;

		//移動状態のときボタンを押されたら
	}
	else
	{
		body.setVelocity(Vec2(0.0,body.getVelocity().y));

		playerState = ePlayerState::idle;
	}
}

void Player::jumpmovement(s3d::detail::XInput_impl controller)
{
	//ジャンプ処理
	if (controller.buttonA.pressed() == true && is_on_ground == true || KeySpace.pressed() == true && is_on_ground == true)
	{
		is_on_ground = false;
		body.setVelocity(Vec2(body.getVelocity().x, -550.0));
	}
}
