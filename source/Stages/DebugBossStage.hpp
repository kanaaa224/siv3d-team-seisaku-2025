# pragma once

# include "Stage.hpp"

class DebugBossStage : public Stage
{
public:
	DebugBossStage();
	~DebugBossStage() = default;

	void update() override;
	void draw() const override;

	static void NewInstance();

private:
	Camera2D camera;

	void initialize();
};
