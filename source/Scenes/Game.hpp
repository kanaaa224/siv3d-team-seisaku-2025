# pragma once

# include "../Common.hpp"

enum StageState
{
	Playing,
	Cleared,
	Gameover
};

// ゲームシーン
class Game : public App::Scene
{
public:
	Game(const InitData& init);

	void update() override;
	void draw() const override;
};
