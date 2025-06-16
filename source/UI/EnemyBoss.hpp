# pragma once
# include "HPBar.hpp"

class EnemyBossUI
{
public:
	EnemyBossUI();
	~EnemyBossUI() = default;

	void update();
	void draw() const;

	void setHP(float num = 0.0f) { hp = num; }
	void setName(String str = U"") { name = str; }

	static EnemyBossUI* GetInstance();

private:
	static EnemyBossUI* instance;

	float hp;

	String name;

	Font font{ 30 };

	HPBar hpBar{ 1000 };
};
