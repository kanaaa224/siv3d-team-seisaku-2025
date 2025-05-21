# pragma once

# include "../Common.hpp"

// デバッグボスシーン
class DebugBoss : public App::Scene
{
public:
	DebugBoss(const InitData& init);

	void update() override;
	void draw() const override;
};
