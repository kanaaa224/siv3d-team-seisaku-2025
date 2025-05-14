# include "Common.hpp"
# include "Scene/Title.hpp"
# include "Scene/Game.hpp"
# include "Scene/Ranking.hpp"

void AssetsRegistration()
{
	// アセットの登録
	TextureAsset::Register({ U"Player_idle", { U"Player" } }, U"../assets/images/player/idle/01_idle_1.png");
}

void Initialize()
{
	Window::Resize(1280, 720);

	Window::SetTitle(U"My Game");
}

void Main()
{
	AssetsRegistration();

	Initialize();

	App manager;

	manager.add<Title>(SceneState::Title);
	manager.add<Game>(SceneState::Game);
	manager.add<Ranking>(SceneState::Ranking);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
