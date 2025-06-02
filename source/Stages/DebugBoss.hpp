# pragma once

# include "../Stage.hpp"

class StageDebugBoss : public Stage
{
public:
	StageDebugBoss();
	~StageDebugBoss() = default;

	void update() override;
	void draw() const override;

	static void NewInstance();

private:
	Camera2D camera;

	void initialize();
};
