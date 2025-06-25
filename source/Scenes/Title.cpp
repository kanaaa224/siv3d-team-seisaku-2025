# include "Title.hpp"
#include "../UI/PlayerHUD.hpp"
# include "../Utils/CustomImageLoader.hpp"

Title::Title(const InitData& init) : IScene{ init }
{
	//Texture読み込み
	m_backgroundTexture1 = TextureAsset(U"Title_Background1");
	m_backgroundTexture2 = TextureAsset(U"Title_Background2");
	m_backgroundTexture3 = TextureAsset(U"Title_Background3");
	m_backgroundTexture4 = TextureAsset(U"Title_Background4");
	title_Logo           = TextureAsset(U"Title_Logo");

	//Sound読み込み
	AudioAsset(U"Battle_BGM").stop();
	AudioAsset(U"End_BGM").stop();
	AudioAsset(U"Title_BGM").setVolume(0.7);
	AudioAsset(U"Title_BGM").play();
	AudioAsset(U"kettei_SE").setVolume(1.0);

	//m_runAnimation = LoadDivGraph(U"Scarerun Idle", Size(150, 45));
	//m_runAnimationFrame = 0;
	//m_runAnimationTime = 0.0;

	//m_runnerAnimation = LoadDivGraph(U"Flot Idle", Size(150, 45));
	//m_runnerAnimationTime = 0.0;
	
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
		AudioAsset(U"kettei_SE").play();
		AudioAsset(U"Title_BGM").stop();
		changeScene(SceneState::Game, 0.5s);
	}
	// マウス左クリックまたはコントローラーのAボタンで決定
	else if(m_exitButton.leftClicked() || (controller.buttonA.down() && m_selectedButtonIndex == 1))
	{
		AudioAsset(U"kettei_SE").play();
		AudioAsset(U"Title_BGM").stop();
		changeScene(SceneState::Credit, 0.5s);
		//System::Exit();
	}

	////アニメーション更新
	//m_runAnimationTime += Scene::DeltaTime();
	//if (m_runAnimationTime >= 0.1)
	//{
	//	m_runAnimationFrame = (m_runAnimationFrame + 1) % m_runAnimation.size();
	//	m_runAnimationTime = 0.0;
	//}

	//m_runnerAnimationTime += Scene::DeltaTime();
	//if (m_runnerAnimationTime >= 0.1) // アニメーション速度（0.1秒ごとにフレーム更新）
	//{
	//	m_runnerAnimationFrame = (m_runnerAnimationFrame + 1) % m_runnerAnimation.size();
	//	m_runnerAnimationTime = 0.0;
	//
	//	 if (x != 840)
	//	 {
	//		 x += 5.0;
	//	 }
	//	 if (x >= 650 && y <= 600)
	//	 {
	//		 y += 8.0;
	//	 }
	//	 if (x == 840)
	//	 {
	//		 x += 0.0;
	//		 y += 0.0;
	//		
	//		 m_runnerAnimation = LoadDivGraph(U"Scarerun Idle", Size(150, 45));
	//	 }
	//}
}

void Title::draw() const
{
		
	m_backgroundTexture4.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture3.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture2.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture1.resized(Scene::Size()).draw(0, 0);


	// タイトル描画
	FontAsset(U"TitleFont")(U"森ノ影").drawAt(TextStyle::OutlineShadow(0.2, ColorF{0.1, 0.1, 0.1}, Vec2{3, 3}, ColorF{0.0, 0.5}), 100, Vec2{640, 200},Palette::Lightgreen);

	//Copyright
	static const Font font{ 16 };
	font(U"© さんぴん茶 2025. All rights reserved.").drawAt(Scene::Center() + Vec2{ 0, (Scene::Height() / 2) - 16 });

	// ボタン描画
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

	////左下にプレイヤー表示
	//if (!m_runAnimation.isEmpty())
	//{
	//	m_runAnimation[m_runAnimationFrame].scaled(2.0).draw(840, Scene::Height() - 120);
	//}

	//if (!m_runnerAnimation.isEmpty())
	//{
	//	// スケールを調整したい場合は .scaled() を追加
	//	//m_runnerAnimation[m_runnerAnimationFrame].draw(m_runnerPos);
	//	m_runnerAnimation[m_runAnimationFrame].scaled(2.0).draw(x,y);
	//}

}
