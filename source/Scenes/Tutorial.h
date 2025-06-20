#pragma once

#include <Siv3D.hpp>
#include "../Characters/Player.hpp"

class Tutorial : public App::Scene
{
public:
	Tutorial(const InitData& init);
	~Tutorial();

	void update() override;
	void draw() const override;

private:
	void initialize();

	Array<String> m_messages;
	size_t m_currentIndex = 0;

	Player* player = nullptr;
	Camera2D camera;
	P2World m_world;

	Texture tutorial_background;
};
