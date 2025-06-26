# include "EnemyBoss.hpp"

EnemyBossUI* EnemyBossUI::GetInstance()
{
	static EnemyBossUI instance;

	return &instance;
}

void EnemyBossUI::update()
{
	hpBar.damage(hpBar.getHP() - hp);

	hpBar.update();
}

void EnemyBossUI::draw() const
{
	font(name).drawAt(Scene::Center() - Vec2{ 0, 200 });

	hpBar.draw({ (Scene::Center() - Vec2{ 200, 175 }), SizeF{ 400, 16 } });
}
