# include "EnemyBoss.hpp"

EnemyBossUI* EnemyBossUI::instance = nullptr;

EnemyBossUI::EnemyBossUI() : hp(0.0f) {}

void EnemyBossUI::update()
{
	hpBar.damage(hpBar.getHP() - hp);

	hpBar.update();
}

void EnemyBossUI::draw() const
{
	font(name).drawAt(Scene::Center() - Vec2{ 0, 200 });

	hpBar.draw({ (Scene::Center() - Vec2{ 150, 175 }), SizeF{ 300, 16 } });
}

EnemyBossUI* EnemyBossUI::GetInstance()
{
	if (instance == nullptr) instance = new EnemyBossUI();

	return instance;
}
