# include "End.hpp"
#include "../UI/PlayerHUD.hpp"

End::End(const InitData& init) : IScene{ init }
{

}

void End::update()
{
	auto controller = XInput(0); //コントローラーを取得

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
	if ((m_startButton.leftClicked() && m_selectedButtonIndex == 0) || (controller.buttonA.down() && m_selectedButtonIndex == 0))
	{
		changeScene(SceneState::Game, 0.5s);
	}
	// マウス左クリックまたはコントローラーのAボタンで決定
	else if ((m_exitButton.leftClicked() && m_selectedButtonIndex == 1) || (controller.buttonA.down() && m_selectedButtonIndex == 1))
	{
		System::Exit();
	}
}

void End::draw() const
{
	Scene::SetBackground(ColorF{ 0.5 });

	// end描画
	FontAsset(U"TitleFont")(U"End").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 50, Vec2{ 640, 200 });

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

		const Font& boldFont = FontAsset(U"Bold");
		boldFont(U"PLAY").drawAt(25, m_startButton.center(), ColorF{ 0.1 });
		boldFont(U"EXIT").drawAt(25, m_exitButton.center(), ColorF{ 0.1 });
	}
}
