# pragma once
# include "HPBar.hpp"

class EnemyBossUI
{
public:
	static EnemyBossUI* GetInstance();

	void update();
	void draw() const;

	void setHP(float num = 0.0f) { hp = num; }
	void setName(String str = U"") { name = str; }

private:
	EnemyBossUI() = default;

	float hp = 0.0f;

	String name;

	Font font{ 30 };

	HPBar hpBar{ 1000 };
};
