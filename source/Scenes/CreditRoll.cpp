#include"CreditRoll.h"

CreditRoll::CreditRoll(const InitData& init) : IScene{ init }
{
	m_scrollX = Scene::Width();
	AudioAsset(U"Credit_BGM").play();
}

void CreditRoll::update()
{
	m_scrollX -= 100 * Scene::DeltaTime(); // 横に左スクロール

	// スキップ(クリックかBボタン)
	if (m_exitButton.leftClicked() || XInput(0).buttonB.down())
	{
		AudioAsset(U"Credit_BGM").stop();
		changeScene(SceneState::Title, 0.5s);
	}

	// 一番最後の文字が画面左端を通過したら終了
	if (m_scrollX < -800) // 調整可能
	{
		AudioAsset(U"Credit_BGM").stop();
		changeScene(SceneState::Title, 0.5s);
	}
}

void CreditRoll::draw() const
{
	Scene::SetBackground(ColorF{ 0.1, 0.1, 0.1 });

	const Font& font = FontAsset(U"TitleFont");
	const double centerY = Scene::Height() / 2.0;

	font(U"CREDIT ROLL").draw(m_scrollX, centerY - 100);
	//font(U"Director: You").draw(m_scrollX + 200, centerY);
	//font(U"Programmer: You").draw(m_scrollX + 600, centerY);
	//font(U"Designer: You").draw(m_scrollX + 1000, centerY);
	//font(U"Sound: You").draw(m_scrollX + 1400, centerY);
	//font(U"Special Thanks: Everyone!").draw(m_scrollX + 1800, centerY);
	//font(U"Thank you for playing!").draw(m_scrollX + 2200, centerY + 100);
}
