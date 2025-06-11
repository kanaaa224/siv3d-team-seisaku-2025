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

	double m_scrollX;
};

