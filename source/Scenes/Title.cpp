# include "Title.hpp"
#include "../UI/PlayerHUD.hpp"
# include "../Utils/CustomImageLoader.hpp"

//Stage
#include "../Stages/1.hpp"
//Leaf
#include "../Objects/Leaf.h"
//Tiem
#include "../Utils/Timer.hpp"

using namespace TimerUtils;

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
	//
	// m_runAnimationTime = 0.0;

	//m_runnerAnimation = LoadDivGraph(U"Flot Idle", Size(150, 45));
	//m_runnerAnimationTime = 0.0;

	titleText_first_alpha  = 0.0f;
	titleText_second_alpha = 0.0f;
	titleText_third_alpha  = 0.0f;
	textAlpha_ct = 0.0;
	textAlphaAnimEndFlg = false;

	textShadow_ct = 0.0;
	shadowPos = Vec2{ 0.0,0.0 };
	shadowAnimEndFlg = false;

	subTitleAlpha = 0.0;
	subTitleAlpha_ct = 0.0;

	createLeaf_ct = 0.0;

	showPopup = false;
	debugFlg = false;
	waitInput = false;
	doOneceTimeOutFlg = false;
	m_selected_PopupButtonIndex = 0;
#ifdef _DEBUG
	debugFlg = true;
#endif // _DEBUG

}

Title::~Title() {}

void Title::update()
{
	Stage::GetInstance()->update();

	if (!textAlphaAnimEndFlg) {
		textAlphaCalc();
	}
	if (!shadowAnimEndFlg) {
		textShadowCalc();
	}
	if (textAlphaAnimEndFlg && shadowAnimEndFlg) {
		subTitleAlphaCalc();
	}
	
	createLeaf();

	//タイマー初期化
	PlayerHUD::GetInstance()->resetTime();

	//ボタン処理（waitTime）
	TimerUtils::Update();
	if (waitInput) {
		if (!doOneceTimeOutFlg) {
			SetTimeout([this] {
				waitInput = false;
				doOneceTimeOutFlg = false;
		    }, 10ms);
			doOneceTimeOutFlg = true;
		}
		
	}

	auto controller = XInput(0); //コントローラーを取得

	if (!debugFlg) {
		// ボタンの更新
		{
			// マウスオーバーとコントローラーのAボタン、Bボタンに対応
			m_startTransition.update((controller.buttonA.pressed() || m_selectedButtonIndex == 0) && !showPopup);
			m_exitTransition.update((controller.buttonB.pressed() || m_selectedButtonIndex == 1) && !showPopup);

			if (controller.isConnected())
			{
				Cursor::RequestStyle(CursorStyle::Hand);
			}
		}

		// D-Padの上下で切り替え
		if (controller.buttonDown.down() && !showPopup)
		{
			m_selectedButtonIndex = (m_selectedButtonIndex + 1) % 2; // 0 -> 1 -> 0...
		}
		else if (controller.buttonUp.down() && !showPopup)
		{
			m_selectedButtonIndex = (m_selectedButtonIndex - 1 + 2) % 2; // 1 -> 0 -> 1...
		}

		// ボタンのクリック処理
		// コントローラーのAボタンで決定
		if (((controller.buttonA.down() && m_selectedButtonIndex == 0)) && !showPopup)
		{
			AudioAsset(U"kettei_SE").play();
			AudioAsset(U"Title_BGM").stop();
			//changeScene(SceneState::Game, 0.5s);
			showPopup = true;
		}
		// コントローラーのAボタンで決定
		else if (((controller.buttonA.down() && m_selectedButtonIndex == 1)) && !showPopup)
		{
			AudioAsset(U"kettei_SE").play();
			AudioAsset(U"Title_BGM").stop();
			changeScene(SceneState::Credit, 0.5s);
			//System::Exit();
		}
	}

	if (showPopup) {
		if (controller.buttonY.down()) {
			showPopup = false;
		}

		popupWindowUpdate();
	}

#ifdef _DEBUG
	if (!waitInput) {
		// ボタンの更新
		{
			// マウスオーバーとコントローラーのAボタン、Bボタンに対応
			m_startTransition.update((m_startButton.mouseOver() || controller.buttonA.pressed() || m_selectedButtonIndex == 0) && !showPopup);
			m_exitTransition.update((m_exitButton.mouseOver() || controller.buttonB.pressed() || m_selectedButtonIndex == 1) && !showPopup);

			if (m_startButton.mouseOver() || m_exitButton.mouseOver() || controller.isConnected())
			{
				Cursor::RequestStyle(CursorStyle::Hand);
			}
		}

		//マウス選択でも枠線切り替わるように
		if (m_startButton.mouseOver() && !showPopup)
		{
			m_selectedButtonIndex = 0;
		}
		else if (m_exitButton.mouseOver() && !showPopup)
		{
			m_selectedButtonIndex = 1;
		}


		// D-Padの上下、またはキーボードの上下矢印キーのみで切り替え
		if ((controller.buttonDown.down() || KeyDown.down()) && !showPopup)
		{
			m_selectedButtonIndex = (m_selectedButtonIndex + 1) % 2; // 0 -> 1 -> 0...
		}
		else if ((controller.buttonUp.down() || KeyUp.down()) && !showPopup)
		{
			m_selectedButtonIndex = (m_selectedButtonIndex - 1 + 2) % 2; // 1 -> 0 -> 1...
		}

		// ボタンのクリック処理
		// マウス左クリックまたはコントローラーのAボタンで決定
		if (((m_startButton.leftClicked() && m_selectedButtonIndex == 0) || (controller.buttonA.down() && m_selectedButtonIndex == 0)) && !showPopup)
		{
			AudioAsset(U"kettei_SE").play();
			AudioAsset(U"Title_BGM").stop();
			//changeScene(SceneState::Game, 0.5s);
			showPopup = true;
		}
		// マウス左クリックまたはコントローラーのAボタンで決定
		else if ((m_exitButton.leftClicked() || (controller.buttonA.down() && m_selectedButtonIndex == 1)) && !showPopup)
		{
			AudioAsset(U"kettei_SE").play();
			AudioAsset(U"Title_BGM").stop();
			changeScene(SceneState::Credit, 0.5s);
		}
	}
#endif // _DEBUG
}

void Title::draw() const
{
	m_backgroundTexture4.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture3.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture2.resized(Scene::Size()).draw(0, 0);
	m_backgroundTexture1.resized(Scene::Size()).draw(0, 0);

	Stage::GetInstance()->draw();

	// タイトル描画
	FontAsset(U"Dot_16")(U"森").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, shadowPos, ColorF{ 0.0, 0.5 }), 100, Vec2{ 550, 200 }, ColorF{Palette::Palegreen,titleText_first_alpha});
	FontAsset(U"Dot_16")(U"ノ").drawAt(TextStyle::OutlineShadow(0.2, ColorF{0.1, 0.1, 0.1}, shadowPos, ColorF{0.0, 0.5}), 100, Vec2{640, 200}, ColorF{ Palette::Palegreen,titleText_first_alpha });
	FontAsset(U"Dot_16")(U"影").drawAt(TextStyle::OutlineShadow(0.2, ColorF{0.1, 0.1, 0.1}, shadowPos, ColorF{0.0, 0.5}), 100, Vec2{730, 200}, ColorF{ Palette::Palegreen,titleText_first_alpha });

	if (textAlphaAnimEndFlg && shadowAnimEndFlg) {
		FontAsset(U"Dot_16")(U"mori no kage").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 25, Vec2{ 640, 270 }, ColorF{ Palette::Yellow,subTitleAlpha });
	}

	//Copyright
	static const Font font{ 16 };

	font(U"© さんぴん茶 2025. All rights reserved.").drawAt(Scene::Center() + Vec2{ 0, (Scene::Height() / 2) - 16 });

	// ボタン描画
	// PLAYボタン
	m_startButton.draw(ColorF{ 1.0, m_startTransition.value() + 0.45 }).drawFrame(1);
	// 選択されている場合は枠の色を変える
	if (m_selectedButtonIndex == 0)
	{
		m_startButton.drawFrame(3, 0, Palette::Orange); // 太めのオレンジの枠
	}

	// EXITボタン
	m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() + 0.45 }).drawFrame(1);
	// 選択されている場合は枠の色を変える
	if (m_selectedButtonIndex == 1)
	{
		m_exitButton.drawFrame(3, 0, Palette::Orange); // 太めのオレンジの枠
	}

	const Font& boldFont = FontAsset(U"Dot_16");
	boldFont(U"PLAY").drawAt(40, m_startButton.center(), ColorF{ 0.1 });
	boldFont(U"END").drawAt(40, m_exitButton.center(), ColorF{ 0.1 });

	//popupWindow
	if (showPopup) {
		Rect{ 0,0,Scene::Width(),Scene::Height() }.draw(ColorF(0, 0, 0, 0.5));
		RoundRect popupRect{ Scene::Width() / 2 - 300,Scene::Height() / 2 - 200,600,400,8 };
		popupRect.draw(ColorF{ Palette::White, }).drawFrame(2);

		const Font& font = FontAsset(U"Dot_16");
		font(U"チュートリアル").drawAt(40, Vec2{popupRect.center().x,popupRect.center().y - 100}, ColorF{0.1});

		m_tutorialButton.draw(ColorF{ Palette::Black, m_tutorialTransition.value() + 0.45 }).drawFrame(1);
		font(U"PLAY").drawAt(40, m_tutorialButton.center(), ColorF{Palette::White});
		m_skipButton.draw(ColorF{ Palette::Black, m_skipTransition.value() + 0.45 }).drawFrame(1);
		font(U"SKIP").drawAt(40, m_skipButton.center(), ColorF{ Palette::White });
		m_backButton.draw(ColorF{ Palette::Black, m_backTransition.value() + 0.45 }).drawFrame(1);
		font(U"✕").drawAt(40, m_backButton.center(), ColorF{ Palette::White });
	}
}

void Title::textAlphaCalc()
{
	//経過時間の更新
	textAlpha_ct += Scene::DeltaTime();

	if (textAlpha_ct >= TITLE_TEXT_ALPHA_CHANGE_TIME) {
		titleText_first_alpha  += 0.1f;
		titleText_second_alpha += 0.1f;
		titleText_third_alpha  += 0.1f;

		if (titleText_first_alpha >= 1.0f) {
			titleText_first_alpha = 1.0f;
		}
		if (titleText_second_alpha >= 1.0f) {
			titleText_second_alpha = 1.0f;
		}
		if (titleText_third_alpha >= 1.0f) {
			titleText_third_alpha = 1.0f;
			textAlphaAnimEndFlg = true;
		}

		textAlpha_ct = 0.0;
	}
}

void Title::textShadowCalc()
{
	textShadow_ct += Scene::DeltaTime();

	if (textShadow_ct >= 0.1) {
		shadowPos += Vec2{ 0.5,0.5 };

		if (shadowPos.x >= 3) {
			shadowPos = Vec2{ 3,3 };
			shadowAnimEndFlg = true;
		}
		textShadow_ct = 0.0;
	}
}

void Title::subTitleAlphaCalc()
{
	subTitleAlpha_ct += Scene::DeltaTime();

	if (subTitleAlpha_ct >= 0.05) {
		subTitleAlpha += 0.1;
		if (subTitleAlpha >= 1.0) {
			subTitleAlpha = 1.0;
		}
		subTitleAlpha_ct = 0.0;
	}
}

void Title::createLeaf()
{
	createLeaf_ct += Scene::DeltaTime();

	if (createLeaf_ct >= 0.5) {//指定した時間が経過したら生成する
		Vec2 createPos = Vec2{ (Random(-10,Scene::Size().x) + 10),0.0};//生成位置
		Stage* stage = Stage::GetInstance();
		stage->createObject<Leaf>(createPos, Random(0, 4), eDropPostion::eRight, Random(10, 30));

		createLeaf_ct = 0.0;
	}
}

void Title::popupWindowUpdate()
{
	auto controller = XInput(0); //コントローラーを取得

	if (!debugFlg) {

	}

#ifdef _DEBUG
	// ボタンの更新
	{
		// マウスオーバーとコントローラーのAボタン、Bボタンに対応
		m_tutorialTransition.update((m_tutorialButton.mouseOver() || controller.buttonA.pressed() || m_selected_PopupButtonIndex == 0));
		m_skipTransition.update((m_skipButton.mouseOver() || controller.buttonB.pressed() || m_selected_PopupButtonIndex == 1));
		m_backTransition.update((m_backButton.mouseOver() || controller.buttonB.pressed() || m_selected_PopupButtonIndex == 2));

		if (m_tutorialButton.mouseOver() || m_skipButton.mouseOver() || controller.isConnected())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}

	//マウス選択でも枠線切り替わるように
	if (m_tutorialButton.mouseOver())
	{
		m_selected_PopupButtonIndex = 0;
	}
	else if (m_skipButton.mouseOver())
	{
		m_selected_PopupButtonIndex = 1;
	}
	else if (m_backButton.mouseOver()) {
		m_selected_PopupButtonIndex = 2;
	}


	// D-Padの左右上下、またはキーボードの左右上下矢印キーのみで切り替え
	if ((controller.buttonLeft.down() || KeyLeft.down()))
	{
		m_selected_PopupButtonIndex = (m_selected_PopupButtonIndex + 1) % 2; // 0 -> 1 -> 0...
	}
	else if ((controller.buttonRight.down() || KeyRight.down()))
	{
		m_selected_PopupButtonIndex = (m_selected_PopupButtonIndex - 1 + 2) % 2; // 1 -> 0 -> 1...
	}
	else if ((controller.buttonUp.down() || KeyUp.down()))
	{
		if (m_selected_PopupButtonIndex == 0 || m_selected_PopupButtonIndex == 1) {
			old_m_selected_PopupButtonIndex = m_selected_PopupButtonIndex;
			m_selected_PopupButtonIndex = 2;
		}
	}
	else if ((controller.buttonDown.down() || KeyDown.down()))
	{
		if (m_selected_PopupButtonIndex == 2) {
			m_selected_PopupButtonIndex = old_m_selected_PopupButtonIndex;
		}
	}

	// ボタンのクリック処理
	// マウス左クリックまたはコントローラーのAボタンで決定
	if (((m_tutorialButton.leftClicked() && m_selected_PopupButtonIndex == 0) || (controller.buttonA.down() && m_selected_PopupButtonIndex == 0)))
	{
		AudioAsset(U"kettei_SE").play();
		//チュートリアルからゲームを始める
		getData().current_stage = 0;
		changeScene(SceneState::Game, 0.5s);

	}
	// マウス左クリックまたはコントローラーのAボタンで決定
	else if ((m_skipButton.leftClicked() && m_selected_PopupButtonIndex == 1) || (controller.buttonA.down() && m_selected_PopupButtonIndex == 1))
	{
		AudioAsset(U"kettei_SE").play();
		//そのままゲームを始める
		getData().current_stage = 1;
		changeScene(SceneState::Game, 0.5s);
	}
	else if ((m_backButton.leftClicked() && m_selected_PopupButtonIndex == 2) || (controller.buttonA.down() && m_selected_PopupButtonIndex == 2))
	{
		AudioAsset(U"kettei_SE").play();
		showPopup = false;
		waitInput = true;
	}
#endif // _DEBUG
}
