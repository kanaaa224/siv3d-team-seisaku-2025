# include "Vaillant.hpp"
# include "Slime.hpp"
# include "Player.hpp"
# include "../Stage.hpp"
# include "../Utils/Timer.hpp"
# include "../Objects/HitBox.hpp"
# include "../Objects/Wall.hpp"

using namespace TimerUtils;
using namespace std::chrono_literals;

Vaillant::Vaillant(P2World& world, const Vec2& position) :
	CharacterBase(world, position),

	start_position(position),

	frameTime   (0.0),
	attack_frame(0.0),

	mirrored  (false),
	discovered(false),
	hostility (false),
	hostiled  (false),
	damaged   (false),
	jumped    (false),
	direction (false),

	die_executed    (false),
	destroy_executed(false),

	forbid_jump(false),
	player_hit (false),

	state       (VaillantState::Idle),
	attack_type (VaillantAttackType::Earthquake),
	attack_state(VaillantAttackState::Preparation),

	currentFrame    (0),
	draw_initialized(false)
{
	body = world.createRect(P2Dynamic, position, size = { VAILLANT_SIZE }, { .density = 0.0, .friction = 0.75 }, { .categoryBits = CollisionCategory::Enemy, .maskBits = CollisionCategory::All & ~CollisionCategory::Player & ~CollisionCategory::Enemy });

	body.addRectSensor(RectF{ -(size / 2), size }, { .categoryBits = CollisionCategory::Enemy, .maskBits = CollisionCategory::Player });

	body.setFixedRotation(true);

	initialize();
}

void Vaillant::initialize()
{
	hp = max_hp = VAILLANT_MAX_HP;
}

void Vaillant::attack(VaillantAttackType type)
{
	attack_type = type;

	state = VaillantState::Attack;

	if (attack_state == VaillantAttackState::Preparation) mirrored = player_position.x <= position.x;

	if (attack_type == VaillantAttackType::Earthquake && attack_state == VaillantAttackState::Preparation)
	{
		attack_frame = frameTime;

		SetTimeout([this] { if (state >= VaillantState::Death) return; AudioAsset(U"Vaillant Attack").playOneShot(); }, 990ms);

		SetTimeout([this] {
			if (state >= VaillantState::Death) return;

			attack_state = VaillantAttackState::Attacking;

			spriteAnimator.setAnimationName(AnimationName::Spark2);
			spriteAnimator.setMask({ 1.0, 1.0, 1.0, 0.5 });
			spriteAnimator.setSize({ 400, 150 });
			spriteAnimator.setPosition(position + Vec2{ (mirrored ? -(size.x / 2) - 200 : (size.x / 2) + 200), size.y / 4 });
			spriteAnimator.stop();
			spriteAnimator.show();
			spriteAnimator.play();
		}, 1500ms);

		SetTimeout([this] { if (state >= VaillantState::Death) return; state = VaillantState::Idle; attack_state = VaillantAttackState::Preparation; }, 2000ms);

		attack_state = VaillantAttackState::Start;
	}

	if (attack_type == VaillantAttackType::Earthquake && attack_state == VaillantAttackState::Attacking)
	{
		Stage::GetInstance()->createObject<HitBox>(position + Vec2{ (mirrored ? -size.x / 2 - 400 : size.x / 2), 0 }, *this);
	}

	if (attack_type == VaillantAttackType::Rush && attack_state == VaillantAttackState::Preparation)
	{
		attack_frame = frameTime;

		SetTimeout([this] { if (state >= VaillantState::Death) return; AudioAsset(U"Vaillant Attack").playOneShot(); }, 1400ms);

		SetTimeout([this] {
			if (state >= VaillantState::Death) return;

			attack_state = VaillantAttackState::Attacking;

			spriteAnimator.setAnimationName(AnimationName::Spark2);
			spriteAnimator.setMask({ 1.0, 1.0, 1.0, 0.5 });
			spriteAnimator.setSize({ 400, 150 });
			spriteAnimator.setPosition(position + Vec2{ 0, size.y / 4 });
			spriteAnimator.stop();
			spriteAnimator.show();
			spriteAnimator.play();
		}, 1500ms);

		attack_state = VaillantAttackState::Start;
	}

	if (attack_type == VaillantAttackType::Rush && attack_state == VaillantAttackState::Preparation || attack_state == VaillantAttackState::Ends)
	{
		mirrored = direction = position.x > player_position.x;
	}

	if (attack_type == VaillantAttackType::Rush && attack_state == VaillantAttackState::Attacking)
	{
		body.applyLinearImpulse(direction ? Vec2{ -VAILLANT_RUSH_POWER, 0 } : Vec2{ VAILLANT_RUSH_POWER, 0 });
	}

	if (attack_type == VaillantAttackType::Rush && attack_state == VaillantAttackState::Ends)
	{
		body.applyLinearImpulse(((direction ? Vec2{ -VAILLANT_WALK_POWER, 0 } : Vec2{ VAILLANT_WALK_POWER, 0 }) / 1.5) * (240 / Profiler::FPS()));
	}

	if (attack_type == VaillantAttackType::Slime)
	{
		Stage::GetInstance()->createObject<Slime>(position - Vec2{ 0, size.y / 2 });

		state = VaillantState::Idle;
	}
}

void Vaillant::update()
{
	frameTime += Scene::DeltaTime();

	if (body) position = body.getPos();

	if (position.y >= (Scene::Height() + 100))
	{
		body.setPos(start_position);
		body.setVelocity({ 0, 0 });
	}

#ifdef _DEBUG
	if (KeyV.pressed())
	{
		if (Key1.down()) heal(10.0f);
		if (Key2.down()) applyDamage(10.0f);
		if (Key3.down()) applyDamage(100.0f);
		if (Key4.down()) hp = 10.0f;
	}
#endif

	spriteAnimator.update();

	if (state >= VaillantState::Death)  return;
	if (state == VaillantState::Attack) return attack(attack_type);

	double distance = position.distanceFrom(player_position);

	if (distance <= 400) discovered = true;

	if (discovered && !hostiled)
	{
		if (!hostility)
		{
			AudioAsset(U"Vaillant Discovery").playOneShot();

			SetTimeout([this] { hostiled = true; }, 1000ms);
		}

		hostility = true;

		state = VaillantState::Idle;

		if (position.x > player_position.x) mirrored = true;

		return;
	}

	if (!discovered && !hostiled)
	{
		distance = position.x - start_position.x;

		direction = (distance > 100) ? true : (distance < -100) ? false : direction;

		body.applyLinearImpulse((direction ? Vec2{ -VAILLANT_WALK_POWER, 0 } : Vec2{ VAILLANT_WALK_POWER, 0 }) * (240 / Profiler::FPS()));
	}

	if (discovered && hostiled)
	{
		distance = position.x - player_position.x;

		if (!jumped)
		{
			direction = (distance > 100) ? true : (distance < -100) ? false : direction;

			body.applyLinearImpulse((direction ? Vec2{ -VAILLANT_WALK_POWER, 0 } : Vec2{ VAILLANT_WALK_POWER, 0 }) * (240 / Profiler::FPS()));
		}

		distance = Abs(distance);

		if (!jumped && distance > 150 && distance <= 700)
		{
			return attack(
				distance <= 400 && distance > 150 ? VaillantAttackType::Earthquake :
				distance <= 600 && distance > 500 ? static_cast<VaillantAttackType>(Random(0, 2)) :
				distance <= 700 && distance > 600 ? VaillantAttackType::Rush : attack_type
			);
		}

		if (!jumped && distance <= 150 && not InRange(body.getVelocity().x, -50.0, 50.0) && !forbid_jump/* || InRange(distance, -1.0, 1.0)*/)
		{
			AudioAsset(U"Vaillant Jump").playOneShot();

			body.applyLinearImpulse(Vec2{ (direction ? -VAILLANT_WALK_POWER : VAILLANT_WALK_POWER) * 100, -VAILLANT_JUMP_POWER });

			spriteAnimator.setAnimationName(AnimationName::Smoke1);
			spriteAnimator.setMask({ 1.0, 1.0, 1.0, 0.5 });
			spriteAnimator.setSize({ 200, 150 });
			spriteAnimator.setPosition({ position.x, (position.y + (size.y / 4)) });
			spriteAnimator.stop();
			spriteAnimator.show();
			spriteAnimator.play();

			jumped = true;
		}

		if (InRange(body.getVelocity().y, -1.0, 1.0) && position.y > start_position.y) jumped = false;
	}

	state = VaillantState::Idle;

	if (not InRange(body.getVelocity().x, -1.0, 1.0))
	{
		state = VaillantState::Move;

		mirrored = !(body.getVelocity().x > 0.0);
	}
}

void Vaillant::draw() const
{
	Vec2 margin;

	SizeF cutoutSize{ 255, 233 };

	double frameDuration = 0.0;
	int    frameCount    = 0;

	ColorF mask{ 1.0, 1.0, 1.0, 1.0 };

	Vec2 shiftAmount{ 0, -16 };

	switch (state)
	{
	case VaillantState::Idle:
	case VaillantState::Move:
	{
		margin = { 86, 81 };

		int marginR = 195;

		frameDuration = 0.085;
		frameCount    = 16;

		currentFrame = static_cast<int>(frameTime / frameDuration) % frameCount;

		if (not InRange(body.getVelocity().y, -1.0, 1.0) || position.y < start_position.y) currentFrame = 12;

		if (currentFrame) margin.x += (marginR + cutoutSize.x) * currentFrame;

		break;
	}

	case VaillantState::Attack:
	{
		switch (attack_type)
		{
		case VaillantAttackType::Earthquake:
		{
			margin = { 86, 81 };

			int marginR = 195;

			frameDuration = 0.15;
			frameCount    = 16;

			double elapsed = frameTime - attack_frame;

			int frameIndex = static_cast<int>(elapsed / frameDuration);

			if (frameIndex >= frameCount) frameIndex = frameCount - 1;

			currentFrame = frameIndex;

			if (currentFrame) margin.x += (marginR + cutoutSize.x) * currentFrame;

			RectF(position + Vec2{ (mirrored ? -size.x / 2 - 400 : size.x / 2), 0 }, SizeF{ 400, 100 }).draw(Arg::top(1.0, 0.0, 0.0, 0.0), Arg::bottom(1.0, 0.0, 0.0, 0.2));

			break;
		}

		case VaillantAttackType::Rush:
		{
			margin = { 86, 81 };

			int marginR = 195;

			frameDuration = attack_state >= VaillantAttackState::Attacked ? 0.15 : 0.01;
			frameCount    = 16;

			currentFrame = static_cast<int>(frameTime / frameDuration) % frameCount;

			if (not InRange(body.getVelocity().y, -1.0, 1.0) || position.y < start_position.y) currentFrame = 12;

			if (currentFrame) margin.x += (marginR + cutoutSize.x) * currentFrame;

			if (attack_state == VaillantAttackState::Start) RectF(position + Vec2{ (mirrored ? -size.x / 2 - Scene::Width() : size.x / 2), 0 }, SizeF{ Scene::Width(), 100}).draw(Arg::top(1.0, 0.0, 0.0, 0.0), Arg::bottom(1.0, 0.0, 0.0, 0.2));

			break;
		}
		}

		break;
	}

	case VaillantState::Death:
	case VaillantState::Destroy:
	{
		margin = { 86, 81 };

		int marginR = 195;

		frameDuration = 0.15;
		frameCount    = 16;

		if (!draw_initialized)
		{
			currentFrame = 0;

			draw_initialized = true;
		}

		if (currentFrame < (frameCount - 1)) currentFrame = static_cast<int>(frameTime / frameDuration) % frameCount;

		if (currentFrame) margin.x += (marginR + cutoutSize.x) * currentFrame;

		break;
	}
	}

	if (damaged) mask = ColorF{ 1.0, 0.0, 0.0, 0.5 };

	switch (state)
	{
	case VaillantState::Destroy:
	{
		constexpr double fadeDuration = 1.5;

		double alpha = Max(0.0, 1.0 - (frameTime / fadeDuration));

		mask = ColorF{ 1.0, 1.0, 1.0, alpha };

		break;
	}
	}

	String assetName;

	switch (state)
	{
	case VaillantState::Idle:
		assetName = U"Vaillant Idle";
		break;

	case VaillantState::Move:
		assetName = U"Vaillant Walk";
		break;

	case VaillantState::Attack:
		switch (attack_type)
		{
		case VaillantAttackType::Earthquake:
			assetName = U"Vaillant Attack";
			break;

		case VaillantAttackType::Rush:
			assetName = U"Vaillant Walk";
			break;
		}

		break;

	case VaillantState::Death:
	case VaillantState::Destroy:
		assetName = U"Vaillant Death";
		break;
	}

	if (assetName) TextureAsset(assetName)(margin, cutoutSize).mirrored(mirrored).drawAt(position + shiftAmount, mask);

	spriteAnimator.draw();

	if (discovered && !hostiled && hostility)
	{
		Vec2 balloonSize{ 40, 40 };

		Vec2 balloonPos = position - Vec2{ balloonSize.x / 2, (size.y / 2) + balloonSize.y + 20 };

		RoundRect balloonRect{ balloonPos, balloonSize, 8 };

		balloonRect.draw({ 0.0, 0.0, 0.0, 0.5 });

		static const Font font{ 28 };

		font(U"!").drawAt(balloonRect.center(), Palette::Red);
	}

#ifdef _DEBUG
	body.drawFrame();

	Print << U"Vaillant HP: " << hp << U" / " << max_hp;
	Print << U"Vaillant State: " << static_cast<int>(state);
	Print << U"Vaillant Mass: " << body.getMass();
	Print << U"Vaillant Distance" << Abs(position.x - player_position.x);
#endif
}

void Vaillant::onHit(ObjectBase& object)
{
	if (Player* player = dynamic_cast<Player*>(&object))
	{
		if (!player_hit)
		{
			if (player->getplayerstate() != ePlayerState::attack &&
				player->getplayerstate() != ePlayerState::avoidance &&
				player->getplayerstate() != ePlayerState::jump_attack &&
				player->getplayerstate() != ePlayerState::jump_avoidance)
			{
				if (state <= VaillantState::Attack)
				{
					if (!damaged) player->applyDamage(jumped ? 5.0f : 10.0f);

					object.getBody().applyLinearImpulse(object.getBody().getPos().x < position.x ? Vec2{ -50, 0 } : Vec2{ 50, 0 });
				}
			}

			SetTimeout([this] { player_hit = false; }, 2000ms);

			player_hit = true;
		}

		if (player->getplayerstate() != ePlayerState::jump_avoidance)
		{
			if (state == VaillantState::Attack && attack_type == VaillantAttackType::Rush && attack_state == VaillantAttackState::Attacking) player->applyDamage(20.0f);
		}
	}

	if (Wall* wall = dynamic_cast<Wall*>(&object))
	{
		if (attack_type == VaillantAttackType::Rush && attack_state == VaillantAttackState::Attacking)
		{
			SetTimeout([this] { if (state >= VaillantState::Death) return; attack_state = VaillantAttackState::Ends; }, 1000ms);
			SetTimeout([this] { if (state >= VaillantState::Death) return; state = VaillantState::Idle; attack_state = VaillantAttackState::Preparation; }, 5000ms);

			applyDamage(0.0f);

			attack_state = VaillantAttackState::Attacked;
		}
	}
}

void Vaillant::onDamaged(float amount)
{
	if (!damaged)
	{
		CharacterBase::onDamaged(amount);

		AudioAsset(U"Vaillant Damage").playOneShot();

		SetTimeout([this] { damaged = false; }, 750ms);

		damaged = true;
	}
}

void Vaillant::destroy()
{
	state = VaillantState::Destroy;

	if (!destroy_executed)
	{
		frameTime = 0.0;

		body.release();

		SetTimeout([this] { ObjectBase::destroy(); }, 2000ms);

		destroy_executed = true;
	}
}

void Vaillant::die()
{
	state = VaillantState::Death;

	if (!die_executed)
	{
		AudioAsset(U"Vaillant Death").playOneShot();

		body.setVelocity({ 0, 0 });

		SetTimeout([this] { CharacterBase::die(); }, 2000ms);

		die_executed = true;
	}
}
