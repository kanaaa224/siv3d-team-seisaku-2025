#pragma once

# include "../Common.hpp"

class CreditRoll : public App::Scene
{
public:
	CreditRoll(const InitData& init);

	void update() override;
	void draw() const override;

private:
	RoundRect m_exitButton{ Arg::center(640, 480), 300, 60, 8 };

	Transition m_exitTransition{ 0.2s, 0.2s };

	// 0: PLAY (スタート), 1: EXIT (終了)
	int m_selectedButtonIndex = 0;

	Texture credit_background;

	double m_scrollY;
};

