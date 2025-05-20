# pragma once
# include <Siv3D.hpp>

class GameUI
{
private:
	Vec2 flame_location;
	Vec2 hp_location;

	Array<String> frameNames = {};

	Array<String> iconNames = {
		
	};

public:
	GameUI();
	~GameUI();

	void update();
	void draw()const;

	static GameUI* GetInstance();

private:
	static GameUI* instance;

	void initialize();
	void finalize();
};
