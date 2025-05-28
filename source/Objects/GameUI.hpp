# pragma once
# include <Siv3D.hpp>

class GameUI
{
public:
	GameUI();
	~GameUI() = default;

	void update();
	void draw()const;

	void setPlayerHP(float hp) { player_hp = hp; }

	static GameUI* GetInstance();

private:
	static GameUI* instance;

	void initialize();

	Vec2 flame_location;
	Vec2 hp_location;

	Array<String> frameNames = {};

	Array<String> iconNames = {};

	const Font fontBitmap;

	int buff_amount;

	float player_hp;
};
