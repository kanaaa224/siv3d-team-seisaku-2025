# pragma once
# include "../Common.hpp"
# include "../Characters/Player.hpp"

class PlayerHUD
{
public:
	PlayerHUD();
	~PlayerHUD() = default;

	void update();
	void draw()const;

	void setPlayerHP(float hp) { player_hp = hp; };
	void setPlayerState(int state) { player_state = state; };
	void setPlayerVel(Vec2 vel) { player_vel = vel; };

	static PlayerHUD* GetInstance();

	// Playerインスタンスを設定する関数
	void setPlayer(Player* player) { m_player = player; }

	Array <TextureRegion>  button;

	TextureRegion image;		//画像描画用
	TextureRegion Abutton;
	TextureRegion Bbutton;
	TextureRegion xbutton;

private:
	static PlayerHUD* instance;

	void initialize();


	Vec2 flame_location;
	Vec2 hp_location;

	Array<String> frameNames = {};

	Array<String> iconNames = {};

	const Font fontBitmap;

	int buff_amount;

	float player_hp;
	float player_state;
	Vec2 player_vel;

	Player* m_player;

	double displayed_hp;
	double hp_decrease_speed;



	// タイマー関連の変数
	bool timerStarted = false;
	double startTime = 0.0;
	Vec2 m_prevPlayerVel = Vec2::Zero(); // 前フレームのプレイヤー速度
	//経過時間
	double m_elapsedTime = 0.0;

	bool GO = false;//フラグ


public:

	//時間リセット関数
	void resetTime();


};
