#pragma once
//親クラス
#include "../CharacterBase.hpp"

class EnemyBase : public CharacterBase
{
public:
	EnemyBase(const Vec2& start_position) : CharacterBase(start_position) {};
	~EnemyBase();

	virtual void update() override;
};

