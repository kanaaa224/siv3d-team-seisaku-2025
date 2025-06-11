# include "Title.hpp"
#include "../UI/PlayerHUD.hpp"

Title::Title(const InitData& init) : IScene{ init }
{
	m_backgroundTexture1 = TextureAsset(U"Title_Background1");
	m_backgroundTexture2 = TextureAsset(U"Title_Background2");
	m_backgroundTexture3 = TextureAsset(U"Title_Background3");
	m_backgroundTexture4 = TextureAsset(U"Title_Background4");

	AudioAsset(U"Battle_BGM").stop();
	AudioAsset(U"Title_BGM").setVolume(0.7);
	AudioAsset(U"Title_BGM").play();
}

Title::~Title()
{

}
	

void Title::update()
{
	//タイマー初期化
	PlayerHUD::GetInstance()->resetTime();

	auto controller = XInput(0); //コントローラーを取得

	// ボタンの更新
	{
		// マウスオーバーとコントローラーのAボタン、Bボタンに対応
		m_startTransition.update(m_startButton.mouseOver() || controller.buttonA.pressed() || m_selectedButtonIndex == 0);
		m_exitTransition.update(m_exitButton.mouseOver() || controller.buttonB.pressed() || m_selectedButtonIndex == 1);

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
		AudioAsset(U"Title_BGM").stop();
		changeScene(SceneState::Game, 0.5s);
	}
	// マウス左クリックまたはコントローラーのAボタンで決定
	else if(m_exitButton.leftClicked() || (controller.buttonA.down() && m_selectedButtonIndex == 1))
	{
		AudioAsset(U"Title_BGM").stop();
		changeScene(SceneState::End, 0.5s);
		//System::Exit();
	}
}

void Title::draw() const
{
		
	m_backgroundTexture4.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture3.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture2.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture1.resized(Scene::Size()).draw(0, 0);


	// タイトル描画
	FontAsset(U"TitleFont")(U"Temple").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 50, Vec2{ 640, 200 });

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
		boldFont(U"END").drawAt(25, m_exitButton.center(), ColorF{ 0.1 });
	}
}
