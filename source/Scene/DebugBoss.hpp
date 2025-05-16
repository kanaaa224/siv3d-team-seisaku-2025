# pragma once

# include "../Common.hpp"

// ランキングシーン
class DebugBoss : public App::Scene
{
public:
	DebugBoss(const InitData& init);

	void update() override;
	void draw() const override;

private:
};
