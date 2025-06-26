# pragma once

# include "../Common.hpp"

class GameClear : public SM::Scene
{
#define TIME_COUNTUP 1.0
#define TIME_DRAW_WAIT 1.0
#define TIME_FONT_MAX_SIZE 60
#define TIME_INIT_POS Vec2{ 640, 380 }

public:
	GameClear(const InitData& init);

	void update() override;
	void draw() const override;

private:
	double sceneTime;

	RoundRect m_startButton{ Arg::center(450, 600), 300, 60, 8 };
	RoundRect m_exitButton{ Arg::center(850, 600), 300, 60, 8 };

	Transition m_startTransition{ 0.2s, 0.2s };
	Transition m_exitTransition{ 0.2s, 0.2s };

	// 0: PLAY (スタート), 1: EXIT (終了)
	int m_selectedButtonIndex = 0;

	double m_inactiveTime = 0.0;

	Texture clear_background;
	Texture score_fream;

	//ランク
	String rank;
	ColorF rankColor;//ランクの色
	double rankSize;

	//タイム加算アニメーション
	bool skipDrawClearTimeFlg;
	double ClearTime;
	double drawClearTime;
	double drawClearTime_ct;
	void clearTimeUpdate();
	bool endClearTimeUpdateFlg;

	//タイムが動くアニメーション
	bool skipMovementTiemFlg;
	double timeSize;
	Vec2 timePos;
	void movementTimeUpdate();
	bool endMovementTimeFlg;
};
