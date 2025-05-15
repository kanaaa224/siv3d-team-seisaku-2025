# pragma once
# include "../Common.hpp"
#include "../Objects/Player.hpp"
#include "../Objects/Stage.hpp"

// ゲームシーン
class Game : public App::Scene
{
public:
	Game(const InitData& init);
	~Game();

	void update() override;
	void draw() const override;

private:

	Player* player;
	Stage* stage;
};
