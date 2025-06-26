# pragma once

# include "../Stage.hpp"

class TutorialStage : public Stage
{
public:

	TutorialStage();
	~TutorialStage() = default;

	void update() override;
	void draw() const override;

	static void NewInstance();

private:

	size_t m_currentIndex = 0;

	//ステージの進捗度計算に必要なワールド座標
	double m_stageStartX;
	double m_stageEndX;//Bossの生成位置と同じにする

	Camera2D camera;

	Array<String> helpTexts;       //説明文リスト
	size_t currentTextIndex = 0;   //現在のテキスト
	Stopwatch autoSkipTimer;       //自動スキップタイマー

	bool isLoading = true; // 初期状態はローディング中

	Vec2 starting_point;
	Vec2 last_point;

	bool gaugeStarted;     // ゲージ開始フラグ
	double gaugeStartTime;

private:
	void initialize();

	Vec2 getGaugePosition(double startX, double endX, double duration, double startTime);

	size_t runAnimationFrame = 0;
	double runAnimationTime = 0.0;

	Array<TextureRegion> run_animation;

	Texture Loding_image;
};

