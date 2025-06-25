# pragma once

# include "../Common.hpp"

class GameOver : public SM::Scene
{
public:
	GameOver(const InitData& init);

	void update() override;
	void draw() const override;

private:
	RoundRect m_startButton{ Arg::center(450, 600), 300, 60, 8 };
	RoundRect m_exitButton{ Arg::center(850, 600), 300, 60, 8 };

	Transition m_startTransition{ 0.2s, 0.2s };
	Transition m_exitTransition{ 0.2s, 0.2s };

	// 0: PLAY (スタート), 1: EXIT (終了)
	int m_selectedButtonIndex = 0;

	double m_inactiveTime = 0.0;


	size_t dieAnimationFrame = 0;
	double dieAnimationTime = 0.0;

	Array<TextureRegion> p_dieanime;
};
