#pragma once

class GameUI
{
private:
	static GameUI* instance;

public:
	GameUI() {};
	~GameUI() {};

	void Initialize();
	void Update();
	void Draw();
	void Finalize();

	static GameUI* GetInstance();
};
