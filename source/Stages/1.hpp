# pragma once

# include "../Stage.hpp"

# define STAGE1_WIDTH 5740

class Stage1 : public Stage
{
public:
	Stage1();
	~Stage1() = default;

	void update() override;
	void draw() const override;

	static void NewInstance();

private:
	Camera2D camera;

	bool BossEreaflg;
	bool excuted;
	bool boss_spawn;

	void initialize();

	//ステージの進捗度計算に必要なワールド座標
	double m_stageStartX;
	double m_stageEndX;//Bossの生成位置と同じにする
};
