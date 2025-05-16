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

	TextureAsset::Register({ U"Player Idle", { U"Player" } }, U"../assets/images/player/idle/01_idle_1.png");
	TextureAsset::Register({ U"Character Frame", { U"UI" } }, U"../assets/images/ui/character_frame.png");
	TextureAsset::Register({ U"Character Icon",  { U"UI" } }, U"../assets/images/ui/character_icon.png");
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
