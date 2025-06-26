# pragma once
# include "../Common.hpp"
# include "../Characters/Player.hpp"
# include "../Characters/Vaillant.hpp"

class PlayerHUD
{
public:
	PlayerHUD();
	~PlayerHUD() = default;

	void update();
	void draw()const;

	// 各種プレイヤー/ボス情報のセッター
	void setPlayerHP(float hp) { player_hp = hp; };
	void setPlayerState(int state) { player_state = state; };
	void setPlayerVel(Vec2 vel) { player_vel = vel; }; // プレイヤーの速度
	void setPlayeravoid(float avoid) { player_avoid = avoid; }; // 回避状態
	void setBossState(VaillantState state) { boss_state = state; };
	void setBossEreaFlg(bool flg) { boss_erea_flg = flg; };
	void setPlayerPosition(Vec2 pos) { player_pos = pos; };// プレイヤーの現在位置

	static PlayerHUD* GetInstance();

	// Playerインスタンスを設定する関数 (必要に応じて残しておく)
	void setPlayer(Player* player) { m_player = player; }
	
	double getClearTime() { return elapsedTime; }

	// UIボタンのテクスチャ領域
	Array <TextureRegion> button;
	TextureRegion image;
	TextureRegion Abutton;
	TextureRegion Bbutton;
	TextureRegion xbutton;

	TextureRegion Avoid_button_image;
	TextureRegion Avoid_button_frame;

private:
	void initialize(); //初期化関数

	//HUD要素の位置
	Vec2 flame_location;
	Vec2 hp_location;

	//バフフレームとアイコンの名前リスト
	Array<String> frameNames = {};
	Array<String> iconNames = {};

	const Font fontBitmap; //ビットマップフォント

	int buff_amount; //バフの量

	//プレイヤーとボスの状態変数
	float player_hp;
	int player_state;
	Vec2 player_vel; //プレイヤーの速度
	float player_avoid;
	VaillantState boss_state;
	bool boss_erea_flg;

	Vec2 player_pos;//プレイヤーの現在位置

	Player* m_player;

	double elapsedTime;
	double displayed_hp;
	double hp_decrease_speed;

	//タイマー関連の変数
	bool timerStarted = false;
	double startTime = 0.0;
	Vec2 m_prevPlayerVel = Vec2::Zero();
	double m_elapsedTime = 0.0; //経過時間

	bool GO = false;//タイマースタートフラグ

	//進捗度UIプレイヤーアニメーション関連
	Array <TextureRegion> run_animation; //アニメーションフレーム
	int8 animation_number;				//現在のアニメーションフレーム番号
	double animation_time;				//現在のアニメーションフレームの経過時間
	double max_animation_time;          //進捗度UIのアニメーションの最大時間（フレームアニメーション速度制御用）

	//各バフのカウントを保持するメンバー変数
	int32 m_buffDamageUpCount = 0;
	int32 m_buffSpeedUpCount = 0;

public:
	void resetTime(); // 時間リセット関数
	bool timerLocked;
	//ゲーム内のプレイヤーのX座標の範囲を定義する変数
	double player_game_world_start_x;
	double player_game_world_end_x;

	// バフの数を設定する関数
	void setBuffDamageUpCount(int32 count);
	void setBuffSpeedUpCount(int32 count);
};
