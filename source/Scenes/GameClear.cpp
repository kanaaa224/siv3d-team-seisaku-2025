# include "GameClear.hpp"
#include "../UI/PlayerHUD.hpp"

GameClear::GameClear(const InitData& init) : IScene{ init }
{
	clear_background = TextureAsset(U"End_Background1");
	score_fream = TextureAsset(U"Score Frame");

	AudioAsset(U"End_BGM").setVolume(0.3);
	AudioAsset(U"Battle_BGM").stop();
	AudioAsset(U"End_BGM").play();
	AudioAsset(U"kettei_SE").setVolume(1.0);

	ClearTime = 0.0;
}

void GameClear::update()
{
	auto controller = XInput(0); //コントローラーを取得

	static bool Oneflg = true;
	if (Oneflg)
	{
		FileOpenByTimer();
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
	if ( controller.buttonRight.down()
		 || KeyRight.down())
	{
		m_selectedButtonIndex = (m_selectedButtonIndex + 1) % 2; // 0 -> 1 -> 0...
	}
	else if (controller.buttonLeft.down()
		|| KeyLeft.down())
	{
		m_selectedButtonIndex = (m_selectedButtonIndex - 1 + 2) % 2; // 1 -> 0 -> 1...
	}


	// ボタンのクリック処理
	// マウス左クリックまたはコントローラーのAボタンで決定
	if ((m_startButton.leftClicked() && m_selectedButtonIndex == 0) || (controller.buttonA.down() && m_selectedButtonIndex == 0))
	{
		AudioAsset(U"kettei_SE").play();
		AudioAsset(U"End_BGM").stop();
		changeScene(SceneState::Game, 0.5s);
	}
	// マウス左クリックまたはコントローラーのAボタンで決定
	else if ((m_exitButton.leftClicked() || (controller.buttonA.down() && m_selectedButtonIndex == 1)))
	{
		AudioAsset(U"End_BGM").stop();
		AudioAsset(U"kettei_SE").play();
		changeScene(SceneState::Title, 0.5s);
	}

}

void GameClear::draw() const
{
	clear_background.resized(Scene::Size()).draw(0, 0);

	Scene::SetBackground(ColorF{ 0.5 });

	// クリア描画
	FontAsset(U"TitleFont")(U"Game Clear").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 640, 100 });
	Rect{ Arg::center(Scene::Center()), 550, 350 }.draw(Palette::White);
	score_fream.resized(450, 100).draw(415, 190);
	FontAsset(U"TitleFont")(U"score").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 40, Vec2{ 640, 220 }, Palette::Orange);
	//スコア描画//
	Print << U"クリアスコア描画" << ClearTime;
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
		boldFont(U"REPLAY").drawAt(25, m_startButton.center(), ColorF{ 0.1 });
		boldFont(U"TITLE").drawAt(25, m_exitButton.center(), ColorF{ 0.1 });
	}
}

void GameClear::FileOpenByTimer()
{
	//ファイルをオープンする
	TextReader reader{ U"../assets/text/ClearTime.txt" };

	//例外スロー確認
	if (!reader)
	{
		throw Error{ U"Failed to open `ClearTime.txt`" };
	}

	//テキストファイルの内容を全て読み込む
	const String text = reader.readAll();

	//String型をdouble型に変換
	ClearTime = Parse<double>(text);
}
