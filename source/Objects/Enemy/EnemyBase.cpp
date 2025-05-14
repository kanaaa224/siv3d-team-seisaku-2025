#include "EnemyBase.hpp"

EnemyBase::EnemyBase(const Vec2& start_position) :
	CharacterBase(start_position)//初期位置
{
}

EnemyBase::~EnemyBase()
{
}

void EnemyBase::update()
{
	//初期化
	initUpdate();

	//ダメージストップ処理
	//Flgが経って何秒経過しているか
	if (damageStopFlg == true) {
		damageStop_ct += Scene::DeltaTime();
		//設定した時間になったらFlgをFalseにする
		if (damageStop_ct >= DAMAGE_STOP_TIME) {
			damageStopFlg = false;
			damageStop_ct = 0;
		}
	}

	//座標の更新
	position += velocity;

#ifdef DEBUG
	if (KeyE.pressed() && Key0.pressed()) {//(E + 0)でhpを0にする
		hp = 0;
	}
	if (KeyE.pressed() && Key1.pressed()) {//(E + 1)でhpを10減らす
		getDamage(10);
	}
#endif // DEBUG
}

void EnemyBase::draw() const
{


#ifdef DEBUG
	//画像ができるまでの仮表示
	Rect(position.x, position.y, size.x, size.y).draw(Palette::Red);
#endif // DEBUG
}

void EnemyBase::animation(float delta_second)
{
}

void EnemyBase::movement(float distance)
{
	if (spawnPosition.x <= position.x) {
		velocity.x = -MOVE_SPEED;
		//ここに画像反転などのFlgなど
		//texture.mirrored();//画像反転？(仮)
	}
	if (spawnPosition.x - distance >= position.x) {
		velocity.x = MOVE_SPEED;
		//ここに画像反転などのFlgなど
	}
}

void EnemyBase::initUpdate()
{
	if (initUpdateFlg == false) {
		position = spawnPosition;//スポーンした場所
		initUpdateFlg = true;
	}
}

void EnemyBase::drawHP() const
{
	//現在のHP割合
	float hp_rate = hp / max_hp;
	float hp_x_size = HP_X_MAXSIZE * hp_rate;

	//HPバーをBoxで描画
	
}

void EnemyBase::getDamage(float damage)
{
	//設定した時間経過後またダメージを受ける
	if (damageStopFlg == false) {
		hp -= damage;
		damageStopFlg = true;
	}

	//HPが０以下なら０にする
	if (hp <= 0) {
		hp = 0;
	}
}
