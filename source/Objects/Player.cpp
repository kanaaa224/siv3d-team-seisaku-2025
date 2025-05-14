#include <Siv3D.hpp>
#include "Player.hpp"

#define VELOCITY 4.0

Player::Player(const Vec2& start_position) : CharacterBase(start_position)
{
	is_on_ground = false;
	playerState = ePlayerState::null;
	playerIndex = NULL;
	enableDeadZone = false;
}

Player::~Player()
{

}

void Player::initialize()
{
	size = Vec2(100.0, 100.0);	//サイズ設定
	is_on_ground = true;		//地面についているか？
	position = Vec2(30, 670);	//初期座標
	playerState = ePlayerState::null;	//待機状態に設定
	playerIndex = 0;			//プレイヤーコントローラー 0番目
	enableDeadZone = false;		//デッドゾーン無効化
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

		//移動量なし
		velocity.x = 0.0;

		if (
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

		break;
	case move: //移動処理

		movement(controller);

		break;
	case jump: //ジャンプ処理

		break;
	case avoidance: //回避処理

		break;
	case attack: //攻撃処理

		break;
	case jump_attack: //ジャンプ攻撃処理

		break;
	case die: //死亡処理

		break;
	default: //例外
		break;
	}

	// 画面左側を超えないようにする
	if (position.x < 16.0f)
	{
		position.x = 16.0f;
	}

	// 移動量計算
	position += velocity;
}

void Player::draw() const
{
	TextureAsset(U"Player_idle").draw(position);
	Print << TextureAsset::IsReady(U"Player_idle");
}

void Player::finalize()
{
}

ePlayerState Player::getplayerstate() const
{
	return playerState;
}

void Player::animation()
{

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
		velocity.x = -VELOCITY;

		// flip_flg = true;

		//移動状態のときボタンを押されたら

	}
	else if(controller.buttonRight.pressed() == true || KeyD.pressed() == true || KeyRight.pressed() == true)
	{
		velocity.x = VELOCITY;

		// flip_flg = false;

		//移動状態のときボタンを押されたら
	}
	else
	{
		velocity.x = 0.0;

		playerState = ePlayerState::idle;
	}
}

//void Player::jumpmovement(s3d::detail::XInput_impl controller)
//{
//}
