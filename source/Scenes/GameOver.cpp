#include "GameOver.h"
#include "../UI/PlayerHUD.hpp"
# include "../Utils/CustomImageLoader.hpp"

GameOver::GameOver(const InitData& init) : IScene{ init }
{
	p_dieanime = LoadDivGraph(U"Player Die", Size(288, 45));

	AudioAsset(U"Battle_BGM").stop();
	AudioAsset(U"GameOver_BGM").setVolume(0.7);
	AudioAsset(U"GameOver_BGM").play();
	AudioAsset(U"kettei_SE").setVolume(1.0);
	dieAnimationFrame = 0;
	dieAnimationTime = 0.0;
}

void GameOver::update()
{
	auto controller = XInput(0); //コントローラーを取得

	dieAnimationTime += Scene::DeltaTime();

	if (dieAnimationTime >= 0.1)
	{
		if (dieAnimationFrame < (p_dieanime.size() - 1))  // 最後のフレームで止める
		{
			++dieAnimationFrame;
		}
		dieAnimationTime = 0.0;
	}

	// ボタンの更新
	{
		// マウスオーバーとコントローラーのAボタン、Bボタンに対応
		m_startTransition.update(m_startButton.mouseOver() || controller.buttonA.pressed() || m_selectedButtonIndex == 0); // 選択中もトランジションを更新
		m_exitTransition.update(m_exitButton.mouseOver() || controller.buttonB.pressed() || m_selectedButtonIndex == 1); // 選択中もトランジションを更新

		if (m_startButton.mouseOver() || m_exitButton.mouseOver() || controller.isConnected())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}

	//マウス選択でも枠線切り替わるように
	if (m_startButton.mouseOver())
	{
		m_selectedButtonIndex = 0;
	}
	else if (m_exitButton.mouseOver())
	{
		m_selectedButtonIndex = 1;
	}


	// D-Padの上下、またはキーボードの上下矢印キーのみで切り替え
	if (controller.buttonRight.down()
		/* || KeyRight.down()*/)
	{
		m_selectedButtonIndex = (m_selectedButtonIndex + 1) % 2; // 0 -> 1 -> 0...
	}
	else if (controller.buttonLeft.down()
		/*|| KeyLeft.down()*/)
	{
		m_selectedButtonIndex = (m_selectedButtonIndex - 1 + 2) % 2; // 1 -> 0 -> 1...
	}

	// ボタンのクリック処理
	// マウス左クリックまたはコントローラーのAボタンで決定
	if (/*(m_startButton.leftClicked() && m_selectedButtonIndex == 0) || */(controller.buttonA.down() && m_selectedButtonIndex == 0))
	{
		AudioAsset(U"kettei_SE").play();
		AudioAsset(U"GameOver_BGM").stop();
		PlayerHUD::GetInstance()->resetTime();
		changeScene(SceneState::Game, 0.5s);
	}
	// マウス左クリックまたはコントローラーのAボタンで決定
	else if ((/*m_exitButton.leftClicked() || */(controller.buttonA.down() && m_selectedButtonIndex == 1)))
	{
		AudioAsset(U"GameOver_BGM").stop();
		AudioAsset(U"kettei_SE").play();
		PlayerHUD::GetInstance()->resetTime();
		changeScene(SceneState::Title, 0.5s);
	}
}

void GameOver::draw() const
{
	//end_background.resized(Scene::Size()).draw(0, 0);

	Scene::SetBackground(ColorF{ 0.1, 0.1, 0.1 });

	// end描画
	FontAsset(U"TitleFont")(U"Game Over").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 640, 150 });

	// ボタン描画
	{
		// PLAYボタン
		m_startButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(1);
		// 選択されている場合は枠の色を変える
		if (m_selectedButtonIndex == 0)
		{
			m_startButton.drawFrame(3, 0, Palette::Orange); // 太めのオレンジの枠
		}

		// EXITボタン
		m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(1);

		// 選択されている場合は枠の色を変える
		if (m_selectedButtonIndex == 1)
		{
			m_exitButton.drawFrame(3, 0, Palette::Orange); // 太めのオレンジの枠
		}

		const Font& boldFont = FontAsset(U"Dot_16");

		boldFont(U"REPLAY").drawAt(40, m_startButton.center(),
			(m_selectedButtonIndex == 0) ? ColorF{ 0.1 } : ColorF{ Palette::White });

		boldFont(U"TITLE").drawAt(40, m_exitButton.center(),
			(m_selectedButtonIndex == 1) ? ColorF{ 0.1 } : ColorF{ Palette::White });


		//左下にプレイヤー表示
		if (!p_dieanime.isEmpty())
		{
			p_dieanime[dieAnimationFrame].scaled(2.0).draw(400, Scene::Height()-380);
		}
	}
}
