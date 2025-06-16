# pragma once

# include "Base.hpp"
# include "../Effects/SpriteAnimator.hpp"

# define VAILLANT_SIZE { 150, 200 }
# define VAILLANT_MAX_HP 1000
# define VAILLANT_WALK_POWER 5.0
# define VAILLANT_JUMP_POWER 3000

enum class VaillantState
{
	Idle,   // 待機中
	Walk,   // 移動中
	Attack, // 攻撃
	Death,  // 死亡
	Destroy // 破壊
};

enum class VaillantAttackType
{
	Earthquake, // 地団駄
	Teleport,   // 瞬間移動
	Tentacles,  // 触手
	Rush        // 突進
};

enum class VaillantAttackState
{
	Preparation, // 攻撃準備中
	Start,       // 攻撃開始
	Attacking,   // 攻撃中
	Ends         // 攻撃終了
};

class Vaillant : public CharacterBase
{
public:
	Vaillant(P2World& world, const Vec2& position = { 0, 0 });
	~Vaillant() = default;

	void update() override;
	void draw() const override;

	void destroy() override;
	void die() override;

	void onHit(ObjectBase& object) override;
	void onDamaged(float amount) override;
	
	void setPlayerPosition(Vec2 pos = { 0, 0 }) { player_position = pos; }
	void setAttackStarted(bool b = false) { hostiled = b; }

	VaillantState getState() { return state; }

private:
	Vec2 position;
	Vec2 start_position;
	Vec2 player_position;
	
	SizeF size;
	
	double frameTime;
	double attack_frame;

	bool mirrored;
	bool hostiled;
	bool damaged;
	bool jumped;
	bool direction;
	bool die_executed;
	bool destroy_executed;
	
	VaillantState state;
	VaillantAttackType attack_type;
	VaillantAttackState attack_state;

	SpriteAnimator spriteAnimator;

	mutable int currentFrame;
	mutable bool draw_initialized;

	void initialize();
	void attack();
};
