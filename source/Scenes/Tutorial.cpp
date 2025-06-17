# include "Tutorial.h"

Tutorial::Tutorial(const InitData& init)
	: IScene{ init }
{
	m_messages =
	{
		U"ようこそ！これはチュートリアルです。",
		U"左スティックまたは A/Dキーで移動します。",
		U"スペースキーでジャンプします。",
		U"攻撃は Eキーまたは。",
		U"準備ができたら、ゲームを始めましょう！"
	};
}

void Tutorial::update()
{
	if (KeyEnter.down() || XInput(0).buttonA.down())
	{
		++m_currentIndex;

		if (m_currentIndex >= m_messages.size())
		{
			changeScene(SceneState::Game, 0.5s); // ゲーム本編へ
		}
	}
}

void Tutorial::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.3, 0.5 });

	const Font& font = FontAsset(U"TitleFont");

	font(U"TUTORIAL").drawAt(Scene::Width() / 2, 80, Palette::White);

	if (m_currentIndex < m_messages.size())
	{
		font(m_messages[m_currentIndex]).drawAt(Scene::Center(), Palette::White);
	}

	FontAsset(U"Small")(U"[ENTER]キー または Aボタンで次へ").drawAt(Scene::Width() / 2, Scene::Height() - 60);
}
