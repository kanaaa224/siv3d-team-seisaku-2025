# include "Common.hpp"
# include "Scene/Title.hpp"
# include "Scene/Game.hpp"
# include "Scene/DebugBoss.hpp"

void AssetsRegistration()
{
	// アセットの登録
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);

	FontAsset(U"TitleFont").setBufferThickness(4);

	TextureAsset::Register({ U"Player Idle", { U"Player" } }, U"../assets/images/player/idle/03_idle.png");
	TextureAsset::Register({ U"Player Run",  { U"Player" } }, U"../assets/images/player/run/run_288_45_8.png");
	TextureAsset::Register({ U"Player Attack", { U"Player" } }, U"../assets/images/player/attack1/atk_288_45.png");
	TextureAsset::Register({ U"Player Roll", { U"Player" } }, U"../assets/images/player/roll/roll_288_45_7.png");

	TextureAsset::Register({ U"Stage 1 Background", { U"Stage" } }, U"../assets/images/stage/stage1/forest.png");
	TextureAsset::Register({ U"Scarerun Idle", { U"Enemy" } }, U"../assets/images/enemy/scarerun/idle.png");

	//GameUI
	TextureAsset::Register({ U"Character Frame", { U"UI" } }, U"../assets/images/ui/character_frame.png");
	TextureAsset::Register({ U"Character Icon",  { U"UI" } }, U"../assets/images/ui/character_icon.png");

	TextureAsset::Register({ U"HP_frame",  { U"UI" } }, U"../assets/images/ui/hp_frame.png");
	TextureAsset::Register({ U"HP_bar",  { U"UI" } }, U"../assets/images/ui/hp_bar_back.png");

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
}

void Initialize()
{
	Window::Resize(1280, 720);

	Window::SetTitle(U""); // TODO
}

void Main()
{
	AssetsRegistration();

	Initialize();

	App manager;

	manager.add<Title>(SceneState::Title);
	manager.add<Game>(SceneState::Game);
	manager.add<DebugBoss>(SceneState::DebugBoss);

	manager.init(SceneState::Title, 0.5s);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
