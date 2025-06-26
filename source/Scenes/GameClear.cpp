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

	//クリアタイム取得
	ClearTime = getData().clearTime;

	//ランク付け
	if (ClearTime <= 30.0) {
		rank = U"S";
		rankColor = ColorF{ Palette::Gold };
	}
	else if (ClearTime >= 31.0 && ClearTime <= 60.0) {
		rank = U"A";
		rankColor = ColorF{ Palette::Red };
	}
	else if (ClearTime >= 61.0 && ClearTime <= 80.0) {
		rank = U"B";
		rankColor = ColorF{ Palette::Blue };
	}
	else {
		rank = U"C";
		rankColor = ColorF{ Palette::Yellow };
	}

	drawClearTime_ct = 0.0;
	drawClearTime = 0.0;
	endClearTimeUpdateFlg = false;
	skipDrawClearTimeFlg = false;

	skipMovementTiemFlg = false;
	timeSize = TIME_FONT_MAX_SIZE;
	timePos = TIME_INIT_POS;
	endMovementTimeFlg = false;

	rankDraw_ct = 0.0;
	rankDrawFlg = false;
	rankSize = RANK_SIZE;
}

void GameClear::update()
{
	sceneTime += Scene::DeltaTime();

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

	//クリアタイムのアニメーション
	if (MouseL.down() && !endClearTimeUpdateFlg) {
		skipDrawClearTimeFlg = true;
	}
	if (skipDrawClearTimeFlg && !endClearTimeUpdateFlg) {
		drawClearTime = ClearTime;
		endClearTimeUpdateFlg = true;
	}
	if (!endClearTimeUpdateFlg && !skipDrawClearTimeFlg) {
		clearTimeUpdate();
	}

	//タイムの移動
	if (endClearTimeUpdateFlg) {
		if (MouseL.down() && !endMovementTimeFlg) {
			skipMovementTiemFlg = true;
		}

		if (skipMovementTiemFlg) {
			timePos.y = 240;
			timeSize = 30;
			endMovementTimeFlg = true;
		}

		if (!endMovementTimeFlg && !skipMovementTiemFlg) {
			movementTimeUpdate();
		}
	}

	//Rankの表示
	if (endMovementTimeFlg) {
		rankAnime();
	}
}

void GameClear::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

	clear_background.resized(Scene::Size()).draw(0, 0);

	Scene::SetBackground(ColorF{ 0.5 });

	// クリア描画
	//FontAsset(U"Dot_16")(U"Game Clear").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 640, 100 });
	Rect{ Arg::center(Scene::Center()), Scene::Size().x, 350}.draw(ColorF(Palette::White, 0.9));
	//score_fream.resized(450, 100).draw(415, 190);
	//FontAsset(U"Dot_16")(U"score").drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1, 0.1, 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 40, Vec2{ 640, 220 }, Palette::Orange);
	//スコア描画//
	FontAsset(U"Dot_16")(U"Time: {:.2f} 秒"_fmt(drawClearTime))
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }),
				timeSize, timePos, Palette::Black);

	//rank表示
	if (endMovementTimeFlg) {
		FontAsset(U"Dot_16")(U"Rank: ")
			.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }),
					100, Vec2{450,380}, Palette::Black);
		if (rankDrawFlg) {
			FontAsset(U"Dot_16")(rank)
				.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.1 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }),
						rankSize, Vec2{ 640,380 }, rankColor);
		}
	}

	// ボタン描画
	{
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
		boldFont(U"REPLAY").drawAt(25, m_startButton.center(), ColorF{ 0.1 });
		boldFont(U"TITLE").drawAt(25, m_exitButton.center(), ColorF{ 0.1 });
	}

#ifdef _DEBUG

	Print << U"クリアスコア描画" << ClearTime;

#endif // DEBUG

}

void GameClear::clearTimeUpdate()
{
	drawClearTime_ct += Scene::DeltaTime();

	double t = Clamp(drawClearTime_ct / TIME_COUNTUP, 0.0, TIME_COUNTUP);
	drawClearTime = ClearTime * t;

	if (drawClearTime == ClearTime) {

		if (drawClearTime_ct >= TIME_COUNTUP + TIME_ENDDRAW_WAIT) {
			endClearTimeUpdateFlg = true;
			skipDrawClearTimeFlg = true;
		}
		
	}
}

void GameClear::movementTimeUpdate()
{
	timePos.y += -1.5;
	timeSize += -0.5;

	if (timePos.y <= 240) {
		timePos.y = 240;
	}
	if (timeSize <= 30) {
		timeSize = 30;
	}

	if (timePos.y == 240 && timeSize == 30) {
		endMovementTimeFlg = true;
		skipMovementTiemFlg = true;
	}
}

void GameClear::rankAnime()
{
	rankDraw_ct += Scene::DeltaTime();

	if (rankDraw_ct >= RANK_DRAW) {
		rankDrawFlg = true;

		if (rank == U"S") {
			rankS_Anim(rankDraw_ct);
		}
		if (rank == U"A") {
			rankA_Anim(rankDraw_ct);
		}
		if (rank == U"B") {
			rankB_Anim(rankDraw_ct);
		}
		if (rank == U"C") {
			rankC_Anim(rankDraw_ct);
		}
	}
}

void GameClear::rankS_Anim(double ct)
{
}

void GameClear::rankA_Anim(double ct)
{
}

void GameClear::rankB_Anim(double ct)
{
}

void GameClear::rankC_Anim(double ct)
{
}

/*void GameClear::FileOpenByTimer()
{
	//ファイルをオープンする
	TextReader reader{ U"../saves/ClearTime.txt" };

	//例外スロー確認
	if (!reader)
	{
		throw Error{ U"Failed to open `ClearTime.txt`" };
	}

	//テキストファイルの内容を全て読み込む
	const String text = reader.readAll();

	//String型をdouble型に変換
	ClearTime = Parse<double>(text);
}*/
