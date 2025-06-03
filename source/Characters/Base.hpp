# pragma once

# include "../Objects/Base.hpp"

class CharacterBase : public ObjectBase
{
public:
	CharacterBase(P2World& world, const Vec2& position) : ObjectBase(world, position) {}
	virtual ~CharacterBase() = default;

	float getHP   () const { return hp; }     // 体力を取得する関数
	float getMaxHP() const { return max_hp; } // 体力の上限（最大値）を取得する関数

	virtual void applyDamage(float amount) { onDamaged(amount); } // ダメージを適用する関数
	virtual void heal       (float amount) { onHealed (amount); } // 回復を行う関数

	virtual void kill() { applyDamage(max_hp); } // キルされたときの関数

protected:
	float hp, max_hp; // 体力

	void addHP(float amount) { hp += amount; hp = std::clamp(hp, 0.0f, max_hp); } // 体力を追加する関数（内部処理用）

	virtual void die() { destroy(); } // 死亡時の関数

	virtual void onDamaged(float amount) { addHP(-amount); if (!hp) die(); } // ダメージを受けたときに呼び出される関数
	virtual void onHealed (float amount) { addHP( amount); }                 // 回復をされたときに呼び出される関数
};
