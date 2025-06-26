# pragma once

# include "../Common.hpp"

class GameClear : public SM::Scene
{
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

	bool skipDrawClearTime;
	double ClearTime;
	double drawClearTime;
	double drawClearTime_ct;
	void clearTimeUpdate();
	bool endClearTimeUpdateFlg;
};
