#pragma once

# include "../Common.hpp"
#include <Siv3D.hpp>


class Tutorial : public App::Scene
{
public:
	Tutorial(const InitData& init);

	void update() override;
	void draw() const override;

private:
	Array<String> m_messages; // チュートリアルメッセージ
	int32 m_currentIndex = 0;
	double m_lastInputTime = 0.0;
};

