#include"CreditRoll.h"
# include "../Utils/CustomImageLoader.hpp"

CreditRoll::CreditRoll(const InitData& init) : IScene{ init }
{
	m_scrollX = Scene::Width();
	AudioAsset(U"Credit_BGM").play();

	m_runAnimation = LoadDivGraph(U"Player Run", Size(288, 45));
	m_runAnimationFrame = 0;
	m_runAnimationTime = 0.0;
}

void CreditRoll::update()
{
	m_scrollX -= 100 * Scene::DeltaTime(); //スクロール

	//アニメーション更新
	m_runAnimationTime += Scene::DeltaTime();
	if (m_runAnimationTime >= 0.1) 
	{
		m_runAnimationFrame = (m_runAnimationFrame + 1) % m_runAnimation.size();
		m_runAnimationTime = 0.0;
	}

	//スキップ(クリックかBボタン)
	if (m_exitButton.leftClicked() || XInput(0).buttonB.down())
	{
		AudioAsset(U"Credit_BGM").stop();
		changeScene(SceneState::Title, 0.5s);
	}

	//一番最後の文字が画面左端を通過したら終了
	if (m_scrollX < -2100) //後で変える
	{
		AudioAsset(U"Credit_BGM").stop();
		changeScene(SceneState::Title, 2.0s);  //タイトルシーンに戻る
	}

}

void CreditRoll::draw() const
{
	Scene::SetBackground(ColorF{ 0.1, 0.1, 0.1 });

	const Font& font = FontAsset(U"TitleFont");
	const double centerY = Scene::Height() / 2.0;

	font(U"CREDIT").draw(m_scrollX, centerY-100);
	font(U"チーム").draw(m_scrollX + 520, centerY-100);
	font(U"さんぴん茶").draw(m_scrollX + 475, centerY);
	font(U"画像").draw(m_scrollX + 1005, centerY-120);
	font(U"itch.io").draw(m_scrollX + 980, centerY-30);
	font(U"ゲームまてりあるず").draw(m_scrollX + 850, centerY + 30);
	font(U"SE").draw(m_scrollX + 1520, centerY - 120);
	font(U"Audiostock").draw(m_scrollX + 1420, centerY-30);
	font(U"効果音ラボ").draw(m_scrollX + 1440, centerY+30);
	font(U"Thank You For Playing!").draw(m_scrollX + 2500, centerY);


		//左下にプレイヤー表示
	if (!m_runAnimation.isEmpty())
	{
		m_runAnimation[m_runAnimationFrame].scaled(2.0).draw(30, Scene::Height() - 120);
	}
}
