#pragma once

# include "../Common.hpp"
#include <Siv3D.hpp>


class CreditRoll : public App::Scene
{
public:
	CreditRoll(const InitData& init);

	void update() override;
	void draw() const override;

private:

	RoundRect m_exitButton{ Arg::center(640, 480), 300, 60, 8 };

	Array<TextureRegion> m_runAnimation;

	size_t m_runAnimationFrame = 0;
	double m_runAnimationTime = 0.0;

	double m_scrollX;
 
};

