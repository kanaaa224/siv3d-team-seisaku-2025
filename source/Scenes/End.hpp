# pragma once

# include "../Common.hpp"


// エンドシーン
class End : public App::Scene
{
public:
	End(const InitData& init);

	void update() override;
	void draw() const override;


private:
	RoundRect m_startButton{ Arg::center(640, 400), 300, 60, 8 };
	RoundRect m_exitButton{ Arg::center(640, 480), 300, 60, 8 };

	Transition m_startTransition{ 0.2s, 0.2s };
	Transition m_exitTransition{ 0.2s, 0.2s };

	// 0: PLAY (スタート), 1: EXIT (終了)
	int m_selectedButtonIndex = 0;

	double m_inactiveTime = 0.0;

	Texture end_background;
};
