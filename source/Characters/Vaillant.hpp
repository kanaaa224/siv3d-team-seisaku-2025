# pragma once

# include "Base.hpp"
# include "../Effects/SpriteAnimator.hpp"

# define VAILLANT_SIZE { 150, 200 }
# define VAILLANT_MAX_HP 1000
# define VAILLANT_WALK_POWER 7.5
# define VAILLANT_JUMP_POWER 3000
# define VAILLANT_RUSH_POWER 500

enum class VaillantState
{
	Idle,   // 待機中
	Move,   // 移動中
	Attack, // 攻撃
	Death,  // 死亡
	Destroy // 破壊
};

enum class VaillantAttackType
{
	Earthquake, // 地団駄
	Rush,       // 突進
	Slime,      // スライム
	Tentacles,  // 触手
	Teleport    // 瞬間移動
};

enum class VaillantAttackState
{
	Preparation, // 攻撃準備中
	Start,       // 攻撃開始
	Attacking,   // 攻撃中
	Attacked,    // 攻撃終了
	Ends         // 攻撃後処理
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
	void setForbidJump    (bool b = false)      { forbid_jump = b; }
	void setAttackStarted (bool b = false)      { hostiled = b; }

	VaillantState       getState()       const { return state; }
	VaillantAttackType  getAttackType()  const { return attack_type; }
	VaillantAttackState getAttackState() const { return attack_state; }

private:
	Vec2 position;
	Vec2 start_position;
	Vec2 player_position;
	Vec2 jumped;
	
	SizeF size;
	
	double frameTime;
	double attack_frame;

	bool mirrored;
	bool discovered;
	bool hostility;
	bool hostiled;
	bool damaged;
	bool direction;
	bool die_executed;
	bool destroy_executed;
	bool forbid_jump;
	bool player_hit;
	
	VaillantState state;
	VaillantAttackType attack_type;
	VaillantAttackState attack_state;

	SpriteAnimator spriteAnimator;

	mutable int currentFrame;
	mutable bool draw_initialized;

	void initialize();
	void attack(VaillantAttackType type);
};
