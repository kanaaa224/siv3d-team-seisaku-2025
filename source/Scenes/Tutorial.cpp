#include "Tutorial.h"
#include "../Characters/Player.hpp"
#include "../Stage.hpp"
#include "../Objects/Ground.hpp"

Player* player = nullptr;

Tutorial::Tutorial(const InitData& init)
	: IScene{ init }
{
	initialize();

	tutorial_background = TextureAsset(U"Stage 1 Background");

	m_messages =
	{
		U"ようこそ！これはチュートリアルです。",
		U"移動:左スティックまたは A/Dキー。",
		U"攻撃:XボタンまたはEキー。",
		U"回避攻撃:BボタンまたはQキー。",
		U"ジャンプ:Aボタンまたはスペースキー。",
		U"ジャンプ攻撃:ジャンプ中にXボタンまたはEキー。",
		U"準備ができたら、ゲームを始めましょう！"
	};
	m_currentIndex = 0;
}

void Tutorial::initialize()
{
	AudioAsset(U"Title_BGM").stop();
	AudioAsset(U"Tutorial_BGM").setVolume(0.6);
	AudioAsset(U"Tutorial_BGM").play();

	// プレイヤーを地面の少し上に配置
	player = new Player(m_world, Vec2{ 640, 500 });

	// カメラは固定
	camera = Camera2D(Vec2{ Scene::Width() / 2, Scene::Height() / 2 }, 1.0, CameraControl::None_);

	// 重力設定
	m_world.setGravity(Vec2{ 0, 980 });

	// 地面（Y=600に幅1500で設置）
	Stage::GetInstance()->createObject<Ground>(Vec2{ 640, 400 });
}

void Tutorial::update()
{
	auto controller = XInput(0);

	if (player)
	{
		player->update();

		Vec2 pos = player->getBody().getPos();

		// プレイヤーが画面外に出ないよう制限
		pos.x = Clamp(pos.x, 0.0, static_cast<double>(Scene::Width()));
		pos.y = Clamp(pos.y, 0.0, static_cast<double>(Scene::Height()));

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

	camera.update(); // カメラは固定
}

void Tutorial::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

	tutorial_background.resized(Scene::Size()).draw(0, 0);

	Scene::SetBackground(ColorF{ 0.1, 0.1, 0.1 });

	const Font& font = FontAsset(U"TitleFont");

	const auto t = camera.createTransformer();

	if (player)
	{
		player->draw();
	}

	if (m_currentIndex < m_messages.size())
	{
		font(m_messages[m_currentIndex]).drawAt(Scene::Center(), Palette::White);
	}

	//FontAsset(U"TitleFont")(U"[ENTER]キー または Aボタンで次へ").drawAt(Scene::Width() / 2, Scene::Height() - 60);
	FontAsset(U"TitleFont")(U"[ENTER] or Aボタンで次へ").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 30, Vec2{ 640, 700 });
}

Tutorial::~Tutorial()
{
	if (player)
	{
		delete player;
		player = nullptr;
	}
}
