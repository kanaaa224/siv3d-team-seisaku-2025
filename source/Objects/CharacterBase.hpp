# pragma once

# include "ObjectBase.hpp"

class CharacterBase : public ObjectBase
{
protected:
	float hp, max_hp; // 体力

	void addHP(float amount) { hp += amount; hp = std::clamp(hp, 0.0f, max_hp); } // 体力を追加する関数（内部処理用）

public:
	CharacterBase(P2World& world, const Vec2& position) : ObjectBase(world, position) {}
	virtual ~CharacterBase() = default;

	float getHP()    const { return hp; }     // 体力を取得
	float getMaxHP() const { return max_hp; } // 体力の上限（最大値）を取得

	virtual void onDamaged(float amount) { addHP(-amount); } // ダメージを受けたときに呼び出される関数
	virtual void onHealed (float amount) { addHP( amount); } // 回復をされたときに呼び出される関数
};
