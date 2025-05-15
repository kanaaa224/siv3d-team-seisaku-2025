# pragma once

#include <Siv3D.hpp>

class GameUI
{
private:

	Vec2 position;

	static GameUI* instance;
	GameUI* gameui;

public:
	GameUI();
	~GameUI();

	void initialize();
	void update();
	void draw()const;
	void finalize();

	static GameUI* GetInstance();
};
