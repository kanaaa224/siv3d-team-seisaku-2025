#pragma once

#include "../EnemyBase.hpp"

class Scarerun : public EnemyBase
{
public:
	Scarerun(const Vec2& start_position);
	~Scarerun();

	void update() override;
	void draw() const override;
};

