# include "Ranking.hpp"

Ranking::Ranking(const InitData& init)
	: IScene{ init }
{

}

void Ranking::update()
{
	if (MouseL.down()) changeScene(SceneState::Title, 0.5s);
}

void Ranking::draw() const
{

}
