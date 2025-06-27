# pragma once

# include "../Common.hpp"
//Confetti
#include "../Effects/Confetti.h"

class GameClear : public SM::Scene
{
#define TIME_COUNTUP 1.0
#define TIME_ENDDRAW_WAIT 1.0
#define TIME_FONT_MAX_SIZE 60
#define TIME_INIT_POS Vec2{ 640, 380 }

#define RANK_DRAW 0.5
#define RANK_SIZE 280
#define RANK_POS_X 640
#define RANK_POS_Y 380

#define CREATE_STAR_TIME 0.2
#define STAR_OFFSET_X 60
#define STAR_OFFSET_Y 80
#define CREATE_NUM 3

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

	//ランク
	String rank;
	ColorF rankColor;
	double rankSize;
	bool rankDrawFlg;
	void rankAnime();
	double rankDraw_ct;
	void rankS_Anim(double ct);
	void rankA_Anim(double ct);
	void rankB_Anim(double ct);
	void rankC_Anim(double ct);

	void rankSizeMove();
	double sizeMove_ct;
	bool endSizeMoveFlg;

	//エフェクト
	//星
	void createRankStarEffect();
	double createStar_ct;
	//煙
	void createSmokeEffect();
	bool createSmokeFlg;
	//紙吹雪
	Confetti confetti_Right{ Vec2(Scene::Size().x - 80.0,Scene::Size().y - 100), 1.8, sin(60)};
	Confetti confetti_Left{ Vec2(80.0,Scene::Size().y - 100), 1.8, sin(-60)};
	bool doOnesConfettiFlg;

	bool hasPlayedKazeSE; //seflag
	Stopwatch timer{ StartImmediately::Yes }; //タイマースイッチ

	bool doOnecePlaySE_1;
	bool doOnecePlaySE_2;
};
