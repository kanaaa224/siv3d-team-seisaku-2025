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

private:
	void initialize();
};

