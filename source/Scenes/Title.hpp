# pragma once

# include "../Common.hpp"

// タイトルシーン
class Title : public App::Scene
{
public:
	Title(const InitData& init);
	~Title();

	void update() override;
	void draw() const override;

private:
	RoundRect m_startButton{ Arg::center(640, 400), 300, 60, 8 };
	RoundRect m_exitButton{ Arg::center(640, 480), 300, 60, 8 };

	Transition m_startTransition{ 0.2s, 0.2s };
	Transition m_exitTransition{ 0.2s, 0.2s };

	// 0: PLAY (スタート), 1: EXIT (終了)
	int m_selectedButtonIndex = 0;

	Texture m_backgroundTexture1;
	Texture m_backgroundTexture2;
	Texture m_backgroundTexture3;
	Texture m_backgroundTexture4;

	//アニメーション用1
	Array<TextureRegion> m_runAnimation;

	size_t m_runAnimationFrame = 0;
	double m_runAnimationTime = 0.0;

	//アニメーション用２
	Array<TextureRegion> m_runAnimation2;

	size_t m_runAnimationFrame2 = 0;
	double m_runAnimationTime2 = 0.0;


};
