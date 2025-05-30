# include "Player.hpp"
# include "../Utils/CustomImageLoader.hpp"
# include "../Stages/Stage.hpp"
# include "../Objects/HitBox.hpp"
# include "../Objects/Enemy/Scarerun/Scarerun.hpp"

#define VELOCITY 150.0	   //移動速度
#define JUMPSPEED 550.0	   //ジャンプ速度
#define DISTANCE 150.0	   //回避距離
#define ITIME	1.5		   //無敵時間

Player::Player(P2World& world, const Vec2& position) : CharacterBase(world, position)
{
	// プレイヤーの干渉フィルター
	filter.categoryBits = 0x0001;		// Playerのカテゴリ設定
	filter.maskBits = 0x0001;			// 敵のカテゴリ設定

	// プレイヤーの当たり判定
	body = world.createRect(P2Dynamic, position, SizeF{ 55, 90 }, P2Material{ .restitution = 0.0, }, P2Filter{ filter.categoryBits }); // 島袋が追記: 物理シミュレーションを行うための箱を生成
	body.setFixedRotation(true);

	is_on_ground = false;
	playerState = ePlayerState::null;
	playerIndex = NULL;
	enableDeadZone = false;
	flip_flg = false;
	animation_number = 0;
	isTriggeredOnce = true;
	isHitFlg = false;
	invincibleDuration = ITIME;

	this->initialize();
}

Player::~Player()
{
	this->finalize();

	idle_animation.clear();
}

void Player::initialize()
{
	is_on_ground = true;		//地面についているか？
	playerState = ePlayerState::null;	//待機状態に設定
	playerIndex = 0;			//プレイヤーコントローラー 0番目
	enableDeadZone = false;		//デッドゾーン無効化
	flip_flg = false;
	hp = 100;
	max_hp = 100;
	//ground_y = 640.0f;  //地面のlocation

	// 分割画像の登録
	idle_animation = LoadDivGraph(U"Player Idle", Size(288, 45));
	run_animation = LoadDivGraph(U"Player Run", Size(288, 45));
	attack_animation = LoadDivGraph(U"Player Attack", Size(288, 45));
	roll_animation = LoadDivGraph(U"Player Roll", Size(288, 45));

	jump_up_animation = LoadDivGraph(U"Player Jump up", Size(288, 46));
	jump_full_animation = LoadDivGraph(U"Player Jump full", Size(288, 60));
	jump_down_animation = LoadDivGraph(U"Player Jump down", Size(288, 60));

	jump_attack_animation = LoadDivGraph(U"Player Jump_Attack", Size(288, 60));

	damage_animation = LoadDivGraph(U"Player Damage", Size(288, 45));
	die_animation = LoadDivGraph(U"Player Die", Size(288, 45));

	image = idle_animation[0];

	body.setVelocity(Vec2(0.0, 0.0)); //移動量設定

	hitStopTimer = 0.0;  //ヒットストップタイマー
	isHitStop = false;   //ヒットストップしたかどうか
	isDamagedOnce = false; // 一度だけダメージ処理を通す

	isAnimeOnce = false;
}

void Player::update()
{
	// 指定したプレイヤーインデックスの XInput コントローラを取得
	auto controller = XInput(playerIndex);

	// ヒットストップ処理
	if (isHitStop == true) {
		
		hitStopTimer -= Scene::DeltaTime();
		if (hitStopTimer <= 0.0) {
			isHitStop = false;
		}
		//return;
	}

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

		isDamagedOnce = false;

		if (isTriggeredOnce == true)
		{
			//移動量なし
			body.setVelocity(Vec2(0.0, body.getVelocity().y));
			isTriggeredOnce = false;
		}
		

		//ジャンプ攻撃有効化
		jump_attack_flg = false;

		animation(idle_animation, 0.1,8,idle);

		//idle状態からボタンを押したごとの処理
		if (//move
			controller.buttonLeft.pressed() == true ||
			controller.buttonRight.pressed() == true ||
			KeyA.pressed() == true ||
			KeyD.pressed() == true ||
			KeyLeft.pressed() == true ||
			KeyRight.pressed() == true||
			std::abs(controller.leftThumbX) > 0.2
			)
		{
			playerState = ePlayerState::move;
		}//jump
		else if (controller.buttonA.down() == true && is_on_ground == true || KeySpace.down() == true && is_on_ground == true)
		{
			animation_number = 0;
			playerState = ePlayerState::jump;
		}//attack
		else if (controller.buttonX.down() == true && jump_attack_flg == false || KeyE.down() == true && jump_attack_flg == false)
		{
			animation_number = 0;
			playerState = ePlayerState::attack;
		}//avoidance
		else if (controller.buttonB.down() == true || KeyQ.down() == true)
		{
			animation_number = 0;
			playerState = ePlayerState::avoidance;
		}///////////////キーを押したらダメージを減らす
		else if (KeyS.down() == true && !isDamagedOnce)////////敵に当たった時にも&& !isInvincibleを追加
		{
			animation_number = 0;
			playerState = ePlayerState::damage;
		}///////////////


		break;
	case move: //移動処理
		isDamagedOnce = false;

		movement(controller);
		animation(run_animation, 0.1,8,idle);
		/////////se追加する場合

		if (controller.buttonA.down() == true && is_on_ground == true || KeySpace.down() == true && is_on_ground == true)
		{
			animation_number = 0;
			playerState = ePlayerState::jump;
		}
		else if (controller.buttonB.down() == true || KeyQ.down() == true)
		{
			animation_number = 0;
			playerState = ePlayerState::avoidance;
		}
		else if (controller.buttonX.down() == true && jump_attack_flg == false || KeyE.down() == true && jump_attack_flg == false)
		{
			animation_number = 0;
			playerState = ePlayerState::attack;
		}

		break;
	case jump: //ジャンプ処理
		isDamagedOnce = false;

		//ジャンプ処理
		jumpmovement(controller);
		////////se追加する場合

		//ジャンプ攻撃
		if (controller.buttonX.down() == true && jump_attack_flg == false || KeyE.down() == true && jump_attack_flg == false)
		{
			playerState = ePlayerState::jump_attack;
		}

		//ジャンプ開始
		if (body.getVelocity().y >= -JUMPSPEED && body.getVelocity().y < 0.0)
		{
			animation(jump_up_animation, 0.1);
		}
		//滞空～降下
		else if (body.getVelocity().y >= 0.0 && body.getVelocity().y < 200.0)
		{
			animation(jump_full_animation, 0.1);
		}
		//降下～着地まで
		else if (body.getVelocity().y >= 200.0 && body.getVelocity().y == 0.0)
		{
			animation(jump_down_animation, 0.1);
		}
		

		//地面についた時の処理
		if (body.getVelocity().y == 0.0) {

			body.setVelocity(Vec2(body.getVelocity().x, body.getVelocity().y));
			is_on_ground = true;
			jump_attack_flg = false;
			animation_number = 0;
			playerState = ePlayerState::idle;
		}
	
		break;
	case avoidance: //回避処理
		isDamagedOnce = false;

		if (flip_flg == true)
		{
			body.setVelocity(Vec2(-DISTANCE, body.getVelocity().y));
			if (animation(roll_animation, 0.1) == true)
			{
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
				}
				else
				{
					playerState = ePlayerState::idle;
				}
			}
		}
		else
		{
			body.setVelocity(Vec2(DISTANCE, body.getVelocity().y));
			if (animation(roll_animation, 0.1) == true)
			{
				//idle状態からボタンを押したごとの処理
				if (//move
					controller.buttonLeft.pressed() == true ||
					controller.buttonRight.pressed() == true ||
					controller.buttonLThumb.pressed() == true ||
					KeyA.pressed() == true ||
					KeyD.pressed() == true ||
					KeyLeft.pressed() == true ||
					KeyRight.pressed() == true
					)
				{
					playerState = ePlayerState::move;
				}
				else
				{
					playerState = ePlayerState::idle;
				}
			}
		}

		break;
	case attack: //攻撃処理
		//isDamagedOnce = false;
		///////////se

		if (flip_flg == true)
		{
			Stage::GetInstance()->createObject<HitBox>(Vec2(body.getPos().x - 98, body.getPos().y - 45));
		}
		else
		{
			Stage::GetInstance()->createObject<HitBox>(Vec2(body.getPos().x + 28, body.getPos().y - 45));
		}
		
		
		if (animation(attack_animation, 0.1) == true)
		{
			playerState = ePlayerState::idle;
		}

		break;
	case jump_attack: //ジャンプ攻撃処理
		jump_attack_flg = true;
		isDamagedOnce = false;

		if (flip_flg == true)
		{
			Stage::GetInstance()->createObject<HitBox>(Vec2(body.getPos().x - 98, body.getPos().y - 45));
		}
		else
		{
			Stage::GetInstance()->createObject<HitBox>(Vec2(body.getPos().x + 28, body.getPos().y - 45));
		}

		if (animation(jump_attack_animation, 0.1)) {
			playerState = ePlayerState::jump;
		}

		break;
	case ePlayerState::die: //死亡処理
		isDamagedOnce = false;
		
		////////se

		//アニメーション
		if (animation(die_animation, 0.1))
		{
			die();
		}

		break;
	case damage:

		

		if (!isDamagedOnce) {
			
			hp -= 10;
			// ヒットストップ開始
			isHitStop = true;
			hitStopTimer = 3.0;  //後で時間調整

			isDamagedOnce = true; // 一度だけ実行する
		}
		////////se

		//アニメーション
		if (animation(damage_animation, 0.1))
		{
			playerState = ePlayerState::idle;
		}

		break;
	default: //例外
		break;
	}

	if (hp <= 0)
	{
		playerState = ePlayerState::die;
	}

	// （仮） 落ちたら戻ってくる
	if (body.getPos().y >= 1000) die();

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

	//無敵中は点滅
	if ((isDamagedOnce && Fmod(Scene::Time(), 0.1) < 0.05)) {
		image.mirrored(flip_flg).resized(size).drawAt(body.getPos(), Palette::Red);
	}

#ifdef _DEBUG

	Print << U"Player HP : " << hp;
	Print << U"Player 座標 : " << body.getPos();
	Print << U"Player 移動量 : " << body.getVelocity();
	Print << U"Player State : " << playerState;
	Print << U"Hit判定 : " << isHitFlg;

#endif // DEBUG
}

void Player::onHit(ObjectBase& object)
{
	//プレイヤーに当たったら
	if (Scarerun* scarerun = dynamic_cast<Scarerun*>(&object)) {
		isHitFlg = true;
		if (!isDamagedOnce)
		{
			playerState =ePlayerState::damage;
		}
	}
	else
	{
		isHitFlg = false;
	}
}

void Player::finalize()
{

}

ePlayerState Player::getplayerstate() const
{
	return playerState;
}

//void Player::setplayerstate(ePlayerState state)
//{
//	playerState = state;
//}

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

bool Player::animation(Array<TextureRegion> image_container, double frame)
{
	animation_time += Scene::DeltaTime();

	if (animation_time >= frame)
	{
		animation_time = 0.0;
		animation_number++;
		if (animation_number >= image_container.size())
		{
			animation_number = 0;
			return true;
		}
		else
		{
			image = image_container[animation_number];
			return false;
		}
	}
	
	return false;
}

void Player::movement(s3d::detail::XInput_impl controller)
{
	// 左スティックのXY軸
	double lx = controller.leftThumbX;
	double ly = controller.leftThumbY;

	//移動処理
	if (
		controller.buttonLeft.pressed() == true ||
		KeyA.pressed() == true ||
		KeyLeft.pressed() == true
		)
	{

		body.setVelocity(Vec2(-VELOCITY, body.getVelocity().y));

		flip_flg = true;

		//移動状態のときボタンを押されたら

	}
	else if(controller.buttonRight.pressed() == true || KeyD.pressed() == true || KeyRight.pressed() == true)
	{

		body.setVelocity(Vec2(VELOCITY, body.getVelocity().y));

		flip_flg = false;

		//移動状態のときボタンを押されたら
	}
	else if (std::abs(lx) > 0.2)  // デッドゾーン 0.2
	{
		// 移動速度をスティック傾きに応じて調整
		body.setVelocity(Vec2(lx * VELOCITY, body.getVelocity().y));

		// 左右の向き判定
		flip_flg = (lx < 0);
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
		body.setVelocity(Vec2(body.getVelocity().x, -JUMPSPEED));
	}
}
