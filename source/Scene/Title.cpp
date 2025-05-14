# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);

	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);
}

void Title::update()
{
	// ボタンの更新
	{
		m_startTransition.update(m_startButton.mouseOver());
		m_rankingTransition.update(m_rankingButton.mouseOver());
		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_startButton.mouseOver() || m_rankingButton.mouseOver() || m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}

	// ボタンのクリック処理
	if (m_startButton.leftClicked()) // ゲームへ
	{
		changeScene(SceneState::Game);
	}
	else if (m_rankingButton.leftClicked()) // ランキングへ
	{
		changeScene(SceneState::Ranking);
	}
	else if (m_exitButton.leftClicked()) // 終了
	{
		System::Exit();
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.5 });

	// タイトル描画
	FontAsset(U"TitleFont")(U"Temple（仮）")
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 50, Vec2{ 400, 100 });

	// ボタン描画
	{
		m_startButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(2);
		m_rankingButton.draw(ColorF{ 1.0, m_rankingTransition.value() }).drawFrame(2);
		m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(2);

		const Font& boldFont = FontAsset(U"Bold");
		boldFont(U"PLAY").drawAt(25, m_startButton.center(), ColorF{ 0.1 });
		boldFont(U"RANKING").drawAt(25, m_rankingButton.center(), ColorF{ 0.1 });
		boldFont(U"EXIT").drawAt(25, m_exitButton.center(), ColorF{ 0.1 });
	}
}
