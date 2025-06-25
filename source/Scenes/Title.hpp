# pragma once

# include "../Common.hpp"

#define TITLE_TEXT_ALPHA_CHANGE_TIME 0.1

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

	

	void textAlphaCalc();
	double textAlpha_ct;
	float titleText_first_alpha; //「森」のアルファ値
	float titleText_second_alpha;//「ノ」のアルファ値
	float titleText_third_alpha; //「影」のアルファ値
	bool textAlphaAnimEndFlg;

	void textShadowCalc();
	double textShadow_ct;
	Vec2 shadowPos;
	bool shadowAnimEndFlg;

	void subTitleAlphaCalc();
	double subTitleAlpha_ct;
	float subTitleAlpha;

	void createLeaf();

	//****IMG****//
	//backGround
	Texture m_backgroundTexture1;
	Texture m_backgroundTexture2;
	Texture m_backgroundTexture3;
	Texture m_backgroundTexture4;
	//タイトルロゴ
	Texture title_Logo;

	//アニメーション用1
	Array<TextureRegion> m_runAnimation;
	size_t m_runAnimationFrame = 0;
	double m_runAnimationTime = 0.0;

	Array<TextureRegion> m_runnerAnimation; // ランナーのアニメーション画像
	int32 m_runnerAnimationFrame;         // 現在のアニメーションフレーム
	double m_runnerAnimationTime;         // アニメーションの時間管理
	Vec2 m_runnerPos;                    // ランナーの現在位置
	int x = 500;
	int y = 280;

};
