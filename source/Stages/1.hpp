# pragma once

# include "../Stage.hpp"

#define STAGE1_WIDTH 5740

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

	void initialize();
};
