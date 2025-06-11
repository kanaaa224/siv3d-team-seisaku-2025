#include "CreditRoll.h"
#include "../UI/PlayerHUD.hpp"

CreditRoll::CreditRoll(const InitData& init) : IScene{ init }
{
	m_scrollY = Scene::Height();

	credit_background = TextureAsset(U"Credit_Background1");

	//AudioAsset(U"End_BGM").setVolume(20);
	AudioAsset(U"End_BGM").stop();
	//AudioAsset(U"End_BGM").play();
}

void CreditRoll::update()
{
	m_scrollY -= 50 * Scene::DeltaTime();

	auto controller = XInput(0); //コントローラーを取得

	// ボタンの更新
	{
		// マウスオーバーとコントローラーのAボタン、Bボタンに対応
		m_exitTransition.update(m_exitButton.mouseOver() || controller.buttonB.pressed() || m_selectedButtonIndex == 1); // 選択中もトランジションを更新

		if (m_exitButton.mouseOver() || controller.isConnected())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}

	//マウス選択でも枠線切り替わるように
	if (m_exitButton.mouseOver())
	{
		m_selectedButtonIndex = 1;
	}


	// D-Padの上下、またはキーボードの上下矢印キーのみで切り替え
	if (controller.buttonDown.down() || KeyDown.down())
	{
		m_selectedButtonIndex = (m_selectedButtonIndex + 1) % 2; // 0 -> 1 -> 0...
	}
	else if (controller.buttonUp.down() || KeyUp.down())
	{
		m_selectedButtonIndex = (m_selectedButtonIndex - 1 + 2) % 2; // 1 -> 0 -> 1...
	}

	// ボタンのクリック処理
	// マウス左クリックまたはコントローラーのAボタンで決定
	if ((m_exitButton.leftClicked() || (controller.buttonA.down() && m_selectedButtonIndex == 1)))
	{
		changeScene(SceneState::Game, 0.5s);
	}

}

void CreditRoll::draw() const
{
	//画像
	//end_background.resized(Scene::Size()).draw(0, 0);

	Scene::SetBackground(ColorF{ 0.5 });

	// end描画
	FontAsset(U"TitleFont")(U"Credit").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 50, Vec2{ 640, 200 });

	// ボタン描画
	{
		// EXITボタン
		m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(1);
		// 選択されている場合は枠の色を変える
		if (m_selectedButtonIndex == 1)
		{
			m_exitButton.drawFrame(3, 0, Palette::Orange); // 太めのオレンジの枠
		}

		const Font& boldFont = FontAsset(U"Bold");
		boldFont(U"EXIT").drawAt(25, m_exitButton.center(), ColorF{ 0.1 });
	}
}
