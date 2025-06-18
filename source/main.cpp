# include "Common.hpp"
# include "Scenes/Title.hpp"
# include "Scenes/Game.hpp"
# include "Scenes/End.hpp"
# include "Scenes/Result.h"
# include "Scenes/GameOver.h"
# include "Scenes/CreditRoll.h"


void AssetsRegistration()
{
	// アセットの登録
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);

	// Font
	FontAsset(U"TitleFont").setBufferThickness(4);

	// Player
	TextureAsset::Register({ U"Player Idle", { U"Player" } }, U"../assets/images/player/idle/03_idle.png");
	TextureAsset::Register({ U"Player Run",  { U"Player" } }, U"../assets/images/player/run/run_288_45_8.png");
	TextureAsset::Register({ U"Player Attack", { U"Player" } }, U"../assets/images/player/attack1/atk_288_45.png");
	TextureAsset::Register({ U"Player Jump Attack2", { U"Player"} }, U"../assets/images/player/jump_attack/jump_attack.png");
	TextureAsset::Register({ U"Player Roll", { U"Player" } }, U"../assets/images/player/roll/roll_288_45_7.png");

	TextureAsset::Register({ U"Player Jump up", { U"Player" } }, U"../assets/images/player/jump_up/jump_up2_2.png");
	TextureAsset::Register({ U"Player Jump full", { U"Player" } }, U"../assets/images/player/jump_full/jump_between_1.png");
	TextureAsset::Register({ U"Player Jump down", { U"Player" } }, U"../assets/images/player/jump_down/03_jump_down_1.png");

	TextureAsset::Register({ U"Player Jump_Attack", { U"Player" } }, U"../assets/images/player/air_attak/air_atk1.png");
	TextureAsset::Register({ U"Player Damage", { U"Player" } }, U"../assets/images/player/take_hit/take_hit_1.png");
	TextureAsset::Register({ U"Player Die", { U"Player" } }, U"../assets/images/player/death/death_1.png");

	//title
	TextureAsset::Register({ U"Title_Background1", { U"Title" } }, U"../assets/images/title/j1.png");
	TextureAsset::Register({ U"Title_Background2", { U"Title" } }, U"../assets/images/title/j2.png");
	TextureAsset::Register({ U"Title_Background3", { U"Title" } }, U"../assets/images/title/j3.png");
	TextureAsset::Register({ U"Title_Background4", { U"Title" } }, U"../assets/images/title/j4.png");

	//End
	TextureAsset::Register({ U"End_Background1", { U"End" } }, U"../assets/images/end/End4.png");

	// Stage
	TextureAsset::Register({ U"Stage 1 Background", { U"Stage" } }, U"../assets/images/stage/stage1/forest.png");
	TextureAsset::Register({ U"Stage Background", { U"Stage" } }, U"../assets/images/stage/stage1/Background.png");

	// Enemy
	TextureAsset::Register({ U"Scarerun Idle Old", { U"Enemy" } }, U"../assets/images/enemy/scarerun/idle.png");//旧画像
	TextureAsset::Register({ U"Scarerun Idle",     { U"Enemy" } }, U"../assets/images/enemy/scarerun/new/Idle.png");
	TextureAsset::Register({ U"Scarerun Attack",   { U"Enemy" } }, U"../assets/images/enemy/scarerun/new/Attack.png");
	TextureAsset::Register({ U"Scarerun Run",      { U"Enemy" } }, U"../assets/images/enemy/scarerun/new/Run.png");
	TextureAsset::Register({ U"Scarerun GetDamage",{ U"Enemy" } }, U"../assets/images/enemy/scarerun/new/Take Hit.png");
	TextureAsset::Register({ U"Scarerun Death",    { U"Enemy" } }, U"../assets/images/enemy/scarerun/new/Death.png");

	TextureAsset::Register({ U"Flot Idle",     { U"Enemy" } }, U"../assets/images/enemy/flotte/Flight.png");
	TextureAsset::Register({ U"Flot Attack",   { U"Enemy" } }, U"../assets/images/enemy/flotte/Attack.png");
	TextureAsset::Register({ U"Flot GetDamage",{ U"Enemy" } }, U"../assets/images/enemy/flotte/Take Hit.png");
	TextureAsset::Register({ U"Flot Death",    { U"Enemy" } }, U"../assets/images/enemy/flotte/Death.png");

	TextureAsset::Register({ U"Ammo",       { U"Enemy" } }, U"../assets/images/enemy/flotte/Energy ball/EnergyBall.png");
	TextureAsset::Register({ U"Ammo Impact",{ U"Enemy" } }, U"../assets/images/enemy/flotte/Energy ball/energyBallImpact.png");
	
	{ // ボス 1
		TextureAsset::Register({ U"Vaillant Idle",   { U"Enemy Boss" } }, U"../assets/images/enemy_boss/vaillant/idle.png");
		TextureAsset::Register({ U"Vaillant Walk",   { U"Enemy Boss" } }, U"../assets/images/enemy_boss/vaillant/walk.png");
		TextureAsset::Register({ U"Vaillant Attack", { U"Enemy Boss" } }, U"../assets/images/enemy_boss/vaillant/attack.png");
		TextureAsset::Register({ U"Vaillant Death",  { U"Enemy Boss" } }, U"../assets/images/enemy_boss/vaillant/death.png");

		TextureAsset::Register({ U"Vaillant Tentacles 1", { U"Enemy Boss" } }, U"../assets/images/enemy_boss/vaillant/attack-2-a.png");

		AudioAsset::Register(U"Vaillant Attack", Audio::Stream, U"../assets/sounds/EnemySE/BossRun.wav");
		AudioAsset::Register(U"Vaillant Death",  Audio::Stream, U"../assets/sounds/EnemySE/BossDies.wav");
		AudioAsset::Register(U"Vaillant Damage", Audio::Stream, U"../assets/sounds/EnemySE/EnemyDamage.wav");
		AudioAsset::Register(U"Vaillant Jump",   Audio::Stream, U"../assets/sounds/PlayerSE/jump.wav");
	}

	{ // バフ
		TextureAsset::Register({ U"Buff",              { U"Buff" } }, U"../assets/images/buf/PotionAsset_20_25.png");
		TextureAsset::Register({ U"AttackPowerBuff",   { U"Buff" } }, U"../assets/images/buf/AttackPower.png");
		TextureAsset::Register({ U"MoveSpeedBuff",     { U"Buff" } }, U"../assets/images/buf/MoveSpeed.png");
	}

	{ // エフェクト
		TextureAsset::Register({ U"Effect 1 1",   { U"Effect" } }, U"../assets/images/effect/1/01.png");
		TextureAsset::Register({ U"Effect 1 2",   { U"Effect" } }, U"../assets/images/effect/1/02.png");
		TextureAsset::Register({ U"Effect 1 29",  { U"Effect" } }, U"../assets/images/effect/1/29.png");
		TextureAsset::Register({ U"Effect 1 35",  { U"Effect" } }, U"../assets/images/effect/1/35.png");
		TextureAsset::Register({ U"Effect 1 14",  { U"Effect" } }, U"../assets/images/effect/1/14.png");
		TextureAsset::Register({ U"Effect 1 27",  { U"Effect" } }, U"../assets/images/effect/1/27.png");
		TextureAsset::Register({ U"Excalimation Mark",  { U"Effect" } }, U"../assets/images/effect/excalimation mark2.png");
		TextureAsset::Register({ U"SmallHit",  { U"Effect" } }, U"../assets/images/effect/Hit/Effect_SmallHit_1_532x528.png");
	}

	//GameUI
	TextureAsset::Register({ U"Character Frame", { U"UI" } }, U"../assets/images/ui/character_frame.png");
	TextureAsset::Register({ U"Character Icon",  { U"UI" } }, U"../assets/images/ui/character_icon.png");

	TextureAsset::Register({ U"HP_frame",  { U"UI" } }, U"../assets/images/ui/hp_frame.png");
	TextureAsset::Register({ U"HP_bar",  { U"UI" } }, U"../assets/images/ui/hp_bar_back.png");
	TextureAsset::Register({ U"HP_bar_Enemy",  { U"UI" } }, U"../assets/images/ui/hp_bar_front.png");

	TextureAsset::Register({ U"time_frame",  { U"UI" } }, U"../assets/images/ui/timer_frame.png");


	TextureAsset::Register({ U"Buf_Attack",  { U"UI" } }, U"../assets/images/ui/buf_icons/buf_attack_power_up.png");
	TextureAsset::Register({ U"Attack_Frame",  { U"UI" } }, U"../assets/images/ui/buf_frames/frame_attack_power.png");
	
	TextureAsset::Register({ U"Buf_defense",  { U"UI" } }, U"../assets/images/ui/buf_icons/buf_Defense_power_up.png");
	TextureAsset::Register({ U"Defense_Frame",  { U"UI" } }, U"../assets/images/ui/buf_frames/frame_Defense_power.png");

	TextureAsset::Register({ U"Buf_Hpup",  { U"UI" } }, U"../assets/images/ui/buf_icons/buf_hp_upper_limit_up.png");
	TextureAsset::Register({ U"Hpup_Frame",  { U"UI" } }, U"../assets/images/ui/buf_frames/frame_hp_upper_limit.png");

	TextureAsset::Register({ U"Buf_recovery",  { U"UI" } }, U"../assets/images/ui/buf_icons/buf_hp_recovery.png");
	TextureAsset::Register({ U"Recovery_Frame",  { U"UI" } }, U"../assets/images/ui/buf_frames/frame_hp_recovery.png");

	TextureAsset::Register({ U"Buf_Movement",  { U"UI" } }, U"../assets/images/ui/buf_icons/buf_movement_speed_up.png");
	TextureAsset::Register({ U"Movement_Frame",  { U"UI" } }, U"../assets/images/ui/buf_frames/frame_movement_speed.png");

	TextureAsset::Register({ U"avoidance_button",  { U"UI" } }, U"../assets/images/ui/button/short_button_animation1.png");

	TextureAsset::Register({ U"Button",  { U"UI" } }, U"../assets/images/ui/x_box_botton/xbox_bigbotton_16_16.png");

	//BGM
	AudioAsset::Register(U"Title_BGM", Audio::Stream, U"../assets/sounds/BGM/title_BGM.wav");//タイトル
	AudioAsset::Register(U"End_BGM", Audio::Stream, U"../assets/sounds/BGM/end_BGM.wav");//エンド
	AudioAsset::Register(U"Battle_BGM", Audio::Stream, U"../assets/sounds/BGM/battle_BGM.wav");//戦闘
	AudioAsset::Register(U"Boss_Battle_BGM", Audio::Stream, U"../assets/sounds/BGM/boss_battle_BGM.wav");//ボス戦闘
	AudioAsset::Register(U"GameOver_BGM", Audio::Stream, U"../assets/sounds/BGM/GameOver_BGM2.wav");//ゲームオーバー
	AudioAsset::Register(U"Credit_BGM", Audio::Stream, U"../assets/sounds/BGM/GameOver_BGM3.wav");

	//playerSE
	AudioAsset::Register(U"player_attack1_SE", Audio::Stream, U"../assets/sounds/PlayerSE/attack.wav");
	AudioAsset::Register(U"player_attack2_SE", Audio::Stream, U"../assets/sounds/PlayerSE/attack2.wav");
	AudioAsset::Register(U"player_attack3_SE", Audio::Stream, U"../assets/sounds/PlayerSE/attack3.wav");
	AudioAsset::Register(U"player_dies_SE", Audio::Stream, U"../assets/sounds/PlayerSE/dies.wav");
	AudioAsset::Register(U"player_jump_SE", Audio::Stream, U"../assets/sounds/PlayerSE/jump.wav");
	AudioAsset::Register(U"player_run1_SE", Audio::Stream, U"../assets/sounds/PlayerSE/run5.wav");
	AudioAsset::Register(U"player_damage_SE", Audio::Stream, U"../assets/sounds/PlayerSE/damage.wav");//playerがダメージをうける

	//EnemySE
	AudioAsset::Register(U"Enemy_dies_SE", Audio::Stream, U"../assets/sounds/EnemySE/EnemyDies.wav");//敵が死ぬ音
	AudioAsset::Register(U"Enemy_damage_SE", Audio::Stream, U"../assets/sounds/EnemySE/EnemyDamage.wav");//Enemyがダメージをうける音
	AudioAsset::Register(U"Enemy_attack_SE", Audio::Stream, U"../assets/sounds/EnemySE/EnemyAttack.wav");//空飛ぶ敵の攻撃音
	AudioAsset::Register(U"Enemy_find_SE", Audio::Stream, U"../assets/sounds/EnemySE/hakkenn2.wav");//敵orプレイヤー発見時SE(hakkenn2.wavの1or2で音変更）

	//SE
	AudioAsset::Register(U"kettei_SE", Audio::Stream, U"../assets/sounds/SE/kettei.wav");
	AudioAsset::Register(U"buff_get_SE", Audio::Stream, U"../assets/sounds/SE/BuffGet.wav");//バフ獲得時SE
	
}

void Initialize()
{
	Window::Resize(1280, 720);

	Window::SetTitle(U"Temple（仮）"); // TODO
}

void Main()
{
	AssetsRegistration();

	Initialize();

	App manager;

	manager.add<Title>(SceneState::Title);
	manager.add<Game>(SceneState::Game);
	manager.add<End>(SceneState::End);
	manager.add<Result>(SceneState::Result);
	manager.add<GameOver>(SceneState::GameOver);
	manager.add<CreditRoll>(SceneState::Credit);

	manager.init(SceneState::Title, 0.5s);

	while (System::Update())
	{
		if (not manager.update()) break;
	}
}
