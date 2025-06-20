#include "Tutorial.h"
#include "../Characters/Player.hpp"
#include "../Stage.hpp"
#include"../Objects/Ground.hpp"

Player* player = nullptr;

Tutorial::Tutorial(const InitData& init)
	: IScene{ init }
{
	initialize();

	tutorial_background = TextureAsset(U"Stage 1 Background");

	m_messages =
	{
		U"ようこそ！これはチュートリアルです。",
		U"左スティックで移動します。",
		U"Xボタンで攻撃。",
		U"Bボタンで回避攻撃。",
		U"Aボタンでジャンプします。",
		U"ジャンプ中にBまたはXでジャンプ攻撃",
		U"準備ができたら、ゲームを始めましょう！"
	};
	m_currentIndex = 0;
}

void Tutorial::initialize()
{
	AudioAsset(U"Title_BGM").stop();
	AudioAsset(U"Tutorial_BGM").setVolume(0.6);
	AudioAsset(U"Tutorial_BGM").play();

	// プレイヤーを画面下方に生成
	player = new Player(m_world, Vec2{ 200, Scene::Height() - 120 });

	// カメラ固定（プレイヤー追従なし）
	camera = Camera2D(Vec2{ Scene::Width() / 2, Scene::Height() / 2 }, 1.0, CameraControl::None_);

	m_world.setGravity(Vec2{ 0, 980 }); // 下方向に重力を設定（px/s^2）

	Stage::GetInstance()->createObject<Ground>(Vec2{ 640, 960 }); // 画面中央下あたりに地面
}

void Tutorial::update()
{
	auto controller = XInput(0);

	if (player)
	{
		player->update();

		// プレイヤーが画面外に出ないように制限
		Vec2 pos = player->getBody().getPos();
		const double left = 0.0;
		const double right = Scene::Width();
		const double bottom = Scene::Height();

		// X方向制限
		if (pos.x < left) pos.x = left;
		else if (pos.x > right) pos.x = right;

		// Y方向制限
		if (pos.y > bottom) pos.y = bottom;

		player->getBody().setPos(pos);
	}

	if (KeyEnter.down() || controller.buttonA.down())
	{
		++m_currentIndex;
		if (m_currentIndex >= m_messages.size())
		{
			AudioAsset(U"Tutorial_BGM").stop();
			changeScene(SceneState::Game, 0.5s);
		}
	}

	camera.update(); // カメラ自体は固定
}

void Tutorial::draw() const
{
	tutorial_background.resized(Scene::Size()).draw(0, 0);

	Scene::SetBackground(ColorF{ 0.1, 0.1, 0.1 });

	const Font& font = FontAsset(U"TitleFont");
	//font(U"チュートリアル").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 80, Vec2{ 640, 100 });

	// カメラ変換開始（ただし固定）
	const auto t = camera.createTransformer();

	if (player)
	{
		player->draw();
	}

	if (m_currentIndex < m_messages.size())
	{
		font(m_messages[m_currentIndex]).drawAt(Scene::Center(), Palette::White);
	}

	FontAsset(U"Small")(U"[ENTER]キー または Aボタンで次へ").drawAt(Scene::Width() / 2, Scene::Height() - 60);
}

Tutorial::~Tutorial()
{
	if (player)
	{
		delete player;
		player = nullptr;
	}
}
