# include "DebugBoss.hpp"

DebugBoss::DebugBoss(const InitData& init)
	: IScene{ init }
{

}

void DebugBoss::update()
{
	if (Key1.down() || MouseL.down()) changeScene(SceneState::Title, 0.5s);
}

void DebugBoss::draw() const
{

}
