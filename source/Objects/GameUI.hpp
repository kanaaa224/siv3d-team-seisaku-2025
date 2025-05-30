# pragma once
# include <Siv3D.hpp>
#include"Player.hpp"

class GameUI
{
public:
	GameUI();
	~GameUI() = default;

	void update();
	void draw()const;
	
	void setPlayerHP(float hp) { player_hp = hp; };
	void setPlayerState(int state) { player_state = state; };

	static GameUI* GetInstance();

	// Playerインスタンスを設定する関数
	void setPlayer(Player* player) { m_player = player; }

	Array <TextureRegion>  button;

	TextureRegion image;		//画像描画用
	TextureRegion Abutton;
	TextureRegion Bbutton;
	TextureRegion xbutton;

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
	float player_state;

	Player* m_player;
};
