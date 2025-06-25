# include "Player.hpp"
# include "../Utils/CustomImageLoader.hpp"
# include "../Stage.hpp"
# include "../Objects/HitBox.hpp"
# include "../Characters/Enemies/Scarerun/Scarerun.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Characters/Enemies/Effect/SmallHitEffect.h"
# include "../Objects/Buff/BuffBase.hpp"

#define VELOCITY 150.0			// 移動速度
#define JUMPSPEED 550.0			// ジャンプ速度
#define DISTANCE 150.0			// 回避距離
#define AVOIDANCE_COOLTIME 5.0 	// 回避クールタイム
#define ITIME	2.0				// 無敵時間
#define ATTACK_RANGE 200.0		// 攻撃距離

#define ENEMY_DAMAGE 50.0		//敵へのダメージ

#define IDLE_ANIM_SPEED 0.1		// 待機アニメーションの切り替え速度
#define MOVE_ANIM_SPEED 0.1		// 移動アニメーションの切り替え速度

#define ATTAK_ANIM_SPEED 0.1	// 攻撃アニメーションの切り替え速度 0.05
#define JUMPATTAK_ANIM_SPEED 0.1// 空中攻撃アニメーションの切り替え速度

#define ROLL_ANIM_SPEED 0.1		// 回避アニメーションの切り替え速度

#define JUMPUP_ANIM_SPEED 0.1	// ジャンプ開始アニメーションの切り替え速度
#define JUMPDOWN_ANIM_SPEED 0.1	// ジャンプ降下アニメーションの切り替え速度

#define DAMAGE_ANIM_SPEED 0.1	// ダメージアニメーションの切り替え速度
#define DIE_ANIM_SPEED 0.1		// 死亡アニメーションの切り替え速度

Player::Player(P2World& world, const Vec2& position) : CharacterBase(world, position)
{
	// プレイヤーの干渉フィルター
	filter.categoryBits = CollisionCategory::Player;		// Playerのカテゴリ設定
	filter.maskBits = CollisionCategory::All;			// 敵のカテゴリ設定

	// プレイヤーの当たり判定
	body = world.createRect(P2Dynamic, position, SizeF{ 30, 90 }, P2Material{ .restitution = 0.0, }, P2Filter{ filter.categoryBits }); // 島袋が追記: 物理シミュレーションを行うための箱を生成
	body.setFixedRotation(true);

	is_on_ground = false;
	playerState = ePlayerState::null;
	playerIndex = NULL;
	enableDeadZone = false;
	flip_flg = false;
	animation_number = 0;
	isTriggeredOnce = true;
	hitStopTimer = 0;
	avoidanceCooldown = 0.0;
	avoidanceCooldownDuration = AVOIDANCE_COOLTIME;
	Damageflg = false;
	alpha = 1.0;
	effect_size = Size(30, 90);

	AudioAsset(U"player_attack1_SE").setVolume(1.0);
	AudioAsset(U"player_attack2_SE").setVolume(1.0);
	AudioAsset(U"player_attack3_SE").setVolume(1.0);
	AudioAsset(U"player_dies_SE").setVolume(2.0);
	AudioAsset(U"player_jump_SE").setVolume(1.5);
	run_se = AudioAsset(U"player_run1_SE").setVolume(10.5).setSpeed(2.0);

	//run_se.setSpeed(2.0);

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

	// 分割画像の登録
	idle_animation = LoadDivGraph(U"Player Idle", Size(288, 45));
	run_animation = LoadDivGraph(U"Player Run", Size(288, 45));
	attack_animation = LoadDivGraph(U"Player Attack", Size(288, 45));
	roll_animation = LoadDivGraph(U"Player Roll", Size(288, 45));

	jump_up_animation = LoadDivGraph(U"Player Jump up", Size(288, 46));
	//jump_full_animation = LoadDivGraph(U"Player Jump full", Size(288, 60));
	jump_down_animation = LoadDivGraph(U"Player Jump down", Size(288, 60));

	jump_attack_animation = LoadDivGraph(U"Player Jump_Attack", Size(288, 60));

	jump_attack_2_animation = LoadDivGraph(U"Player Jump Attack2", Size(288, 45));

	damage_animation = LoadDivGraph(U"Player Damage", Size(288, 45));
	die_animation = LoadDivGraph(U"Player Die", Size(288, 45));

	image = idle_animation[0];

	body.setVelocity(Vec2(0.0, 0.0)); //移動量設定

	hitStopTimer = ITIME;  //ヒットストップタイマー
	isHitStop = false;   //ヒットストップしたかどうか

	movement_speed = 0.0;
	attack_power = 30.f;
	wall_hit = true;
	enemyHit = false;
}

void Player::update()
{
	// 指定したプレイヤーインデックスの XInput コントローラを取得
	auto controller = XInput(playerIndex);

#ifdef _DEBUG

	// デバッグ用 //////////////////////////////////////////////////////

	if (KeyS.down()) hp -= 90;

	if (KeyL.down()) body.setPos(Vec2(5000.0, body.getPos().y));

	//////////////////////////////////////////////////////////////////////

#endif //

	if (body)
	{
		pos = body.getPos();
	}

	if (hp <= 0 && is_on_ground == true)
	{
		body.release();
		isTriggeredOnce = true;
		playerState = ePlayerState::die;
	}

	//回避のクールタイム減らす
	if (avoidanceCooldown > 0.0)
	{
		avoidanceCooldown -= Scene::DeltaTime();
	}

	//spriteAnimator.update();

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

		if (isTriggeredOnce == true)
		{
			//移動量なし
			body.setVelocity(Vec2(0.0, body.getVelocity().y));
			isTriggeredOnce = false;
		}

		//ジャンプ攻撃有効化
		jump_attack_flg = false;

		enemyHit = false;

		animation(idle_animation, IDLE_ANIM_SPEED,8,idle);

		//spriteAnimator.setAnimationName(AnimationName::SpawnEffect);
		//spriteAnimator.setMask({ 1.0, 1.0, 1.0, 1.0 });
		//spriteAnimator.setSize({ 120, 120 });
		//spriteAnimator.setPosition({ Scene::Width() / 2, Scene::Height() / 2 });
		//spriteAnimator.setLooping(true);
		//spriteAnimator.setAnimationSpeed(0.1);
		////spriteAnimator.stop();
		//spriteAnimator.show();
		//spriteAnimator.play();


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
			AudioAsset(U"player_attack2_SE").playOneShot();//通常攻撃SE
			animation_number = 0;
			playerState = ePlayerState::attack;
			AudioAsset(U"player_run1_SE").stop();//走るSE停止
		}//avoidance
		else if (controller.buttonB.down() == true || KeyQ.down() == true && avoidanceCooldown <= 0.0)
		{
			AudioAsset(U"player_attack3_SE").play();//回避攻撃SE
			animation_number = 0;
			playerState = ePlayerState::avoidance;
			avoidanceCooldown = avoidanceCooldownDuration; // クールタイム設定
			AudioAsset(U"player_run1_SE").stop();//走るSE停止
		}///////////////キーを押したらダメージを減らす
		else if (isHitStop == true && hitStopTimer >= ITIME)////////敵に当たった時にも&& !isInvincibleを追加
		{
			animation_number = 0;
			playerState = ePlayerState::damage;
		}


		break;
	case move: //移動処理
		enemyHit = false;
		movement(controller);
		animation(run_animation, MOVE_ANIM_SPEED,8,idle);
		/////////se追加する場合
		// 
		//地面にいたら走るSE
		if (is_on_ground == true)
		{
			run_se.setVolume(10.5).setSpeed(2.0).play();
		}
		
		if (controller.buttonA.down() == true && is_on_ground == true || KeySpace.down() == true && is_on_ground == true)
		{
			run_se.stop();
			animation_number = 0;
			playerState = ePlayerState::jump;
		}
		else if (controller.buttonX.down() == true && jump_attack_flg == false || KeyE.down() == true && jump_attack_flg == false)
		{
			AudioAsset(U"player_attack2_SE").playOneShot();//通常攻撃
			animation_number = 0;
			playerState = ePlayerState::attack;
		}
		else if (controller.buttonB.down() == true || KeyQ.down() == true && avoidanceCooldown <= 0.0)
		{
			AudioAsset(U"player_attack3_SE").playOneShot();//回避攻撃
			animation_number = 0;
			playerState = ePlayerState::avoidance;
			avoidanceCooldown = avoidanceCooldownDuration; // クールタイム設定
		}
		break;
	case jump: //ジャンプ処理
		enemyHit = false;
		//ジャンプ処理
		jumpmovement(controller);
		////////se追加する場合

		if (KeyS.down() == true)
		{
			hp -= 90;
		}

		//ジャンプ中左右移動可能
		if (controller.buttonLeft.pressed() == true ||
			controller.buttonRight.pressed() == true ||
			KeyA.pressed() == true ||
			KeyD.pressed() == true ||
			KeyLeft.pressed() == true ||
			KeyRight.pressed() == true ||
			std::abs(controller.leftThumbX) > 0.2)
		{
			movement(controller);
		}

		//ジャンプ攻撃
		if (controller.buttonX.down() == true && jump_attack_flg == false || KeyE.down() == true && jump_attack_flg == false)
		{
			AudioAsset(U"player_attack2_SE").playOneShot();//ジャンプ中攻撃SE
			animation_number = 0;
			playerState = ePlayerState::jump_attack;
		}
		//空中回避
		else if (controller.buttonB.down() == true || KeyQ.down() == true && avoidanceCooldown <= 0.0)
		{
			AudioAsset(U"player_attack3_SE").play();//回避攻撃SE
			animation_number = 0;
			playerState = ePlayerState::jump_avoidance;
			avoidanceCooldown = avoidanceCooldownDuration; // クールタイム設定
		}

		//ジャンプ開始
		if (body.getVelocity().y >= -JUMPSPEED && body.getVelocity().y < 0.0)
		{
			animation(jump_up_animation, JUMPUP_ANIM_SPEED);
		}
		//下降中
		else if (body.getVelocity().y > 50.0)
		{
			
			animation(jump_down_animation, JUMPDOWN_ANIM_SPEED);
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

		if (flip_flg == true)
		{
			body.setVelocity(Vec2(-DISTANCE, body.getVelocity().y));
			Stage::GetInstance()->createObject<HitBox>(Vec2{ body.getPos().x - 168, body.getPos().y - 45 }, *this);
			if (enemyHit)
			{
				Stage::GetInstance()->createObject<SmallHitEffect>(Vec2{ body.getPos().x - 130, body.getPos().y - 1 }, *this);
			}
			if (animation(jump_attack_animation, JUMPATTAK_ANIM_SPEED) == true)
			{
				body.setVelocity(Vec2(0.0, body.getVelocity().y)); //

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
					animation_number = 0;
					playerState = ePlayerState::move;
				}
				else if (controller.buttonA.down() == true && is_on_ground == true || KeySpace.down() == true && is_on_ground == true)
				{
					animation_number = 0;
					playerState = ePlayerState::jump;
				}
				else
				{
					animation_number = 0;
					playerState = ePlayerState::idle;
				}
			}
		}
		else
		{
			body.setVelocity(Vec2(DISTANCE, body.getVelocity().y));
			Stage::GetInstance()->createObject<HitBox>(Vec2{ body.getPos().x + 16, body.getPos().y - 45 }, *this);
			if (enemyHit)
			{
				Stage::GetInstance()->createObject<SmallHitEffect>(Vec2{ body.getPos().x + 120, body.getPos().y - 1 }, *this);
			}
			if (animation(jump_attack_animation, JUMPATTAK_ANIM_SPEED) == true)
			{
				body.setVelocity(Vec2(0.0, body.getVelocity().y));
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
					animation_number = 0;
					playerState = ePlayerState::move;
				}
				else if(controller.buttonA.down() == true && is_on_ground == true || KeySpace.down() == true && is_on_ground == true)
				{
					animation_number = 0;
					playerState = ePlayerState::jump;
				}
				else
				{
					animation_number = 0;
					playerState = ePlayerState::idle;
				}
			}
		}

		break;
	case attack: //攻撃処理
		///////////se

		if (flip_flg == true)  ///攻撃当たり判定
		{
			body.setVelocity(Vec2(0.0, body.getVelocity().y));
			Stage::GetInstance()->createObject<HitBox>(Vec2{ body.getPos().x - 111, body.getPos().y - 45 }, *this);
			if (enemyHit)
			{
				Stage::GetInstance()->createObject<SmallHitEffect>(Vec2{ body.getPos().x - 100, body.getPos().y }, *this);
			}
		}
		else
		{
			body.setVelocity(Vec2(0.0, body.getVelocity().y));
			Stage::GetInstance()->createObject<HitBox>(Vec2{ body.getPos().x + 15, body.getPos().y - 45 }, *this);
			if (enemyHit)
			{
				Stage::GetInstance()->createObject<SmallHitEffect>(Vec2{ body.getPos().x + 90, body.getPos().y }, *this);
			}
		}
		
		
		if (animation(attack_animation, ATTAK_ANIM_SPEED) == true)
		{
			animation_number = 0;
			playerState = ePlayerState::idle;
		}

		break;
	case jump_attack: //ジャンプ攻撃処理
		jump_attack_flg = true;

		if (flip_flg == true)  ///攻撃当たり判定
		{
			//body.setVelocity(Vec2(-15.0, 0.0));
			body.setVelocity(Vec2(0.0, 600.0));
			Stage::GetInstance()->createObject<HitBox>(Vec2{ body.getPos().x - 111, body.getPos().y - 45 }, *this);
			if (enemyHit)
			{
				Stage::GetInstance()->createObject<SmallHitEffect>(Vec2{ body.getPos().x - 100, body.getPos().y }, * this);
			}
		}
		else
		{
			//body.setVelocity(Vec2(15.0, 0.0));
			body.setVelocity(Vec2(0.0, 600.0));
			Stage::GetInstance()->createObject<HitBox>(Vec2{ body.getPos().x + 15, body.getPos().y - 45 }, *this);
			if (enemyHit)
			{
				Stage::GetInstance()->createObject<SmallHitEffect>(Vec2{ body.getPos().x + 90, body.getPos().y }, * this);
			}
		}

		if (animation(jump_attack_2_animation, 0.09)) {
			animation_number = 0;
			playerState = ePlayerState::jump;
		}

		break;

	case jump_avoidance:

		if (flip_flg == true)
		{
			body.setVelocity(Vec2(-DISTANCE, 0.0));
			Stage::GetInstance()->createObject<HitBox>(Vec2{ body.getPos().x - 168, body.getPos().y - 45 }, *this);
			if (enemyHit)
			{
				Stage::GetInstance()->createObject<SmallHitEffect>(Vec2{ body.getPos().x - 130, body.getPos().y - 1 }, * this);
			}
			if (animation(jump_attack_animation, JUMPATTAK_ANIM_SPEED) == true)
			{
				body.setVelocity(Vec2(0.0, body.getVelocity().y)); //

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
					movement(controller);
				}

				playerState = ePlayerState::jump;
			}
		}
		else
		{
			body.setVelocity(Vec2(DISTANCE, 0.0));
			Stage::GetInstance()->createObject<HitBox>(Vec2{ body.getPos().x + 16, body.getPos().y - 45 }, *this);
			if (enemyHit)
			{
				Stage::GetInstance()->createObject<SmallHitEffect>(Vec2{ body.getPos().x + 120, body.getPos().y - 1 }, * this);
			}
			if (animation(jump_attack_animation, JUMPATTAK_ANIM_SPEED) == true)
			{
				body.setVelocity(Vec2(0.0, body.getVelocity().y));
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
					movement(controller);
				}

				playerState = ePlayerState::jump;
			}
		}

		break;

	case damage:
		enemyHit = false;
		////////se
		Damageflg = true;
		//アニメーション
		if (animation(damage_animation, DAMAGE_ANIM_SPEED))
		{
			animation_number = 0;
			Damageflg = false;
			if (playerState == ePlayerState::jump || playerState == ePlayerState::jump_attack)
			{
				playerState = ePlayerState::jump;
			}
			else
			{
				playerState = ePlayerState::idle;
			}
		}

		break;

	case ePlayerState::die: //死亡処理
		enemyHit = false;
		////////se
		AudioAsset(U"player_dies_SE").play();
		die();

		break;
	default: //例外
		break;
	}

	// ヒットストップ処理
	if (isHitStop == true) {

		hitStopTimer -= Scene::DeltaTime();
		// 画像の透明度を設定 7.0を増やすと点滅速度が速くなる
		alpha = (1.0 + Sin(Scene::Time() * 7.0 * Math::Pi)) * 0.5;
		if (hitStopTimer <= 0.0) {
			isHitStop = false;
			alpha = 1.0;
			hitStopTimer = ITIME;
		}
	}

	// （仮） 落ちたら戻ってくる
	if (body.getPos().y >= 1000) CharacterBase::die();
	// 移動量計算
	body.setPos(body.getPos() + body.getVelocity() *  Scene::DeltaTime());
}

void Player::draw() const
{
	Vec2 size = Vec2(288.0 * 2, 45.0 * 2);

	// 画像を滑らかに補完
	const ScopedRenderStates2D rs{ SamplerState::ClampNearest };
	
	image.mirrored(flip_flg).resized(size).drawAt(pos, ColorF{ 1.0, 1.0, 1.0, alpha });

	//無敵中は点滅
	if ((Damageflg == true && Fmod(Scene::Time(), 0.1) < 0.05)) {
		image.mirrored(flip_flg).resized(size).drawAt(pos, Palette::Red);
	}

	//spriteAnimator.draw();
	
#ifdef _DEBUG

	body.drawFrame(1.0, ColorF(Palette::Blue));
	image.mirrored(flip_flg).resized(size).drawAt(pos, ColorF{ 1.0, 1.0, 1.0, alpha});

	Print << U"Player HP : " << hp;
	Print << U"Player 座標 : " << body.getPos();
	Print << U"Player 移動量 : " << body.getVelocity();
	Print << U"Player State : " << playerState;
	Print << U"Player 壁 : " << wall_hit;

#endif // DEBUG
}

void Player::onHit(ObjectBase& object)
{
	if (Ground* ground = dynamic_cast<Ground*>(&object))
	{
		is_on_ground = true;
	}
	else
	{
		is_on_ground = false;
	}

	if (Wall* wall = dynamic_cast<Wall*>(&object))
	{
		wall_hit = true;
		//body.applyLinearImpulse(Vec2(-100.0, 0.0));
		/*if (!flip_flg)
		{
			body.applyLinearImpulse(Vec2(1.0, 0.0));
		}
		else if(flip_flg)
		{
			body.applyLinearImpulse(Vec2(-100.0, 0.0));
		}*/
	}
	else
	{
		wall_hit = false;
	}
}

void Player::onDamaged(float amount)
{
	if (isHitStop == false)
	{
		addHP(-amount);
		AudioAsset(U"player_damage_SE").play();
		isHitStop = true;
		animation_number = 0;
		playerState = ePlayerState::damage;
	}
}

void Player::destroy()
{
	ObjectBase::destroy();
}

void Player::die()
{
	if (isTriggeredOnce == true)
	{
		//移動量なし
		body.setVelocity(Vec2(0.0, body.getVelocity().y));
		isTriggeredOnce = false;
	}

	//アニメーション
	if (animation(die_animation, DIE_ANIM_SPEED))
	{
		//2秒待ったらにしたい

		CharacterBase::die();
	}
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
		if (wall_hit == false)
		{
			body.setVelocity(Vec2(-VELOCITY - movement_speed, body.getVelocity().y));
		}

		flip_flg = true;

		//移動状態のときボタンを押されたら

	}
	else if(controller.buttonRight.pressed() == true || KeyD.pressed() == true || KeyRight.pressed() == true)
	{

		if (wall_hit == false)
		{
			body.setVelocity(Vec2(VELOCITY + movement_speed, body.getVelocity().y));
		}

		flip_flg = false;

		//移動状態のときボタンを押されたら
	}
	else if (std::abs(lx) > 0.2)  // デッドゾーン 0.2
	{
		if (wall_hit == false)
		{
			// 移動速度をスティック傾きに応じて調整
			//body.setVelocity(Vec2(lx * VELOCITY + movement_speed, body.getVelocity().y));
			double speedBoost = movement_speed * (lx >= 0 ? 1.0 : -1.0);
			body.setVelocity(Vec2(lx * VELOCITY + speedBoost, body.getVelocity().y));
		}

		// 左右の向き判定
		flip_flg = (lx < 0);
	}
	else
	{
		body.setVelocity(Vec2(0.0, body.getVelocity().y));

		playerState = ePlayerState::idle;
	}
}

void Player::jumpmovement(s3d::detail::XInput_impl controller)
{
	//ジャンプ処理
	if (controller.buttonA.pressed() == true && is_on_ground == true || KeySpace.pressed() == true && is_on_ground == true)
	{
		is_on_ground = false;
		body.setVelocity(Vec2(body.getVelocity().x /*+ movement_speed*/, -JUMPSPEED));
		AudioAsset(U"player_jump_SE").play();//ジャンプSE
	}
}
