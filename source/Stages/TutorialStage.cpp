# include "TutorialStage.hpp"
# include "../Objects/StageBackground.hpp"
# include "../Objects/Ground.hpp"
# include "../Objects/Wall.hpp"
# include "../Objects/HitBox.hpp"
# include "../Characters/Player.hpp"

TutorialStage::TutorialStage()
{
	initialize();
}

void TutorialStage::initialize()
{
	//BGM
	/*AudioAsset(U"Title_BGM").stop();
	AudioAsset(U"Battle_BGM").setVolume(0.7);
	AudioAsset(U"Battle_BGM").play();*/

	//ステージオブジェクト
	createObject<StageBackground>();
	createObject<Ground>(Vec2{ Scene::Width(), (Scene::Height() + 5) });
	createObject<Wall>(Vec2{ -5, 500 });
	createObject<Wall>(Vec2{ Scene::Width(), 500});

	//for (int8 i = 1; i < 6; i++)
	//{
	//	//敵
	//	createObject<Scarerun>(Vec2{ 700 * i, Scene::Height() - 45 });
	//	createObject<Flot>(Vec2{ 800 * i ,500 });
	//}

	//プレイヤー
	createObject<Player>(Vec2{ 100, 650 });
}

void TutorialStage::update()
{
	Stage::update();

	auto _objects_ = objects;

	for (const auto& object : _objects_)
	{
		if (HitBox* hitBox = dynamic_cast<HitBox*>(object))
		{
			hitBox->destroy();
		}
	}
	
	if (KeyEnter.down() || Gamepad(0).isConnected() && Gamepad(0).buttons[7].down())
	{
		sceneData().current_stage = 1;

		sceneChange(SceneState::Game, 0.5s);
	}
}

void TutorialStage::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

#ifdef _DEBUG

	Print << U"オブジェクト数: " << objects.size();

#endif

	{
		Stage::draw();
	}

	FontAsset(U"TitleFont")(U"Enterキーでスキップ可能").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ Scene::Width() / 2, Scene::Height() / 2 });
}

void TutorialStage::NewInstance()
{
	if (instance != nullptr) delete instance;

	instance = new TutorialStage();
}
