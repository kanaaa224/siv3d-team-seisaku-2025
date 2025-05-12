#include "GameUI.h"

// 静的メンバ変数定義 nullptrで初期化
GameUI* GameUI::instance = nullptr;

void GameUI::Initialize()
{
	//
}

void GameUI::Update()
{
	//
}

void GameUI::Draw()
{ 
	//
}

void GameUI::Finalize()
{
	//
}

GameUI* GameUI::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new GameUI();
	}

	return instance;
}
