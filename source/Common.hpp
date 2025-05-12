# pragma once
# include <Siv3D.hpp> // Siv3D v0.6.16

// シーンのステート
enum class State
{
	Title,
	Game,
	Ranking,
};

// 共有するデータ
struct GameData
{
	// 直前のゲームのスコア
	int32 lastScore = 0;

	// ハイスコア
	Array<int32> highScores = { 10, 8, 6, 4, 2 };
};

using App = SceneManager<State, GameData>;
