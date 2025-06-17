# include "Vaillant.hpp"
# include "Player.hpp"
# include "../Stage.hpp"
# include "../Utils/Timer.hpp"
# include "../Objects/HitBox.hpp"

using namespace TimerUtils;
using namespace std::chrono_literals;

Vaillant::Vaillant(P2World& world, const Vec2& position) :
	CharacterBase(world, position),

	start_position(position),

	frameTime   (0.0),
	attack_frame(0.0),

	mirrored (false),
	hostiled (false),
	damaged  (false),
	jumped   (false),
	direction(false),

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
	body = world.createRect(P2Dynamic, position, size = { VAILLANT_SIZE }, { .density = 0.0, .friction = 0.75 }, { .categoryBits = CollisionCategory::Enemy, .maskBits = CollisionCategory::All & ~CollisionCategory::Player });

	body.addRectSensor(RectF{ -(size / 2), size }, { .categoryBits = CollisionCategory::Enemy, .maskBits = CollisionCategory::Player });

	body.setFixedRotation(true);

	initialize();
}

void Vaillant::initialize()
{
	max_hp = VAILLANT_MAX_HP;

	hp = max_hp;
}

void Vaillant::attack()
{
	if (attack_type == VaillantAttackType::Earthquake && attack_state == VaillantAttackState::Preparation)
	{
		attack_frame = frameTime;

		SetTimeout([this] { AudioAsset(U"Vaillant Attack").playOneShot(); }, 990ms);

		SetTimeout([this] {
			attack_state = VaillantAttackState::Attacking;

			spriteAnimator.setAnimationName(AnimationName::Spark2);
			spriteAnimator.setMask({ 1.0, 1.0, 1.0, 0.5 });
			spriteAnimator.setSize({ 400, 150 });
			spriteAnimator.setPosition(position + Vec2{ (mirrored ? -(size.x / 2) - 200 : (size.x / 2) + 200), size.y / 4 });
			spriteAnimator.stop();
			spriteAnimator.show();
			spriteAnimator.play();
		}, 1500ms);

		SetTimeout([this] { state = VaillantState::Idle; attack_state = VaillantAttackState::Preparation; }, 2000ms);

		attack_state = VaillantAttackState::Start;
	}

	if (attack_type == VaillantAttackType::Earthquake && attack_state == VaillantAttackState::Attacking)
	{
		Stage::GetInstance()->createObject<HitBox>(position + Vec2{ (mirrored ? -size.x / 2 - 400 : size.x / 2), 0 }, *this);
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

	if (state >= VaillantState::Death) return;

	double distance = position.distanceFrom(player_position);

	if (distance <= 400) hostiled = true;

	if (hostiled)
	{
		if (state == VaillantState::Attack) return attack();

		distance = position.x - player_position.x;

		direction = (distance > 10) ? true : (distance < -10) ? false : direction;

		body.applyLinearImpulse((direction ? Vec2{ -VAILLANT_WALK_POWER, 0 } : Vec2{ VAILLANT_WALK_POWER, 0 }) * (240 / Profiler::FPS()));

		distance = Abs(distance);

		if (!jumped && distance <= 300 && !forbid_jump)
		{
			if (Random(0, 1))
			{
				state = VaillantState::Attack;

				attack_type = static_cast<VaillantAttackType>(Random(0, 0));

				mirrored = player_position.x <= position.x ? true : false;

				return;
			}

			AudioAsset(U"Vaillant Jump").playOneShot();

			body.applyLinearImpulse(Vec2{ 0, -VAILLANT_JUMP_POWER });

			spriteAnimator.setAnimationName(AnimationName::Smoke1);
			spriteAnimator.setMask({ 1.0, 1.0, 1.0, 0.5 });
			spriteAnimator.setSize({ 200, 150 });
			spriteAnimator.setPosition({ position.x, (position.y + (size.y / 4)) });
			spriteAnimator.stop();
			spriteAnimator.show();
			spriteAnimator.play();

			jumped = true;
		}

		if (InRange(body.getVelocity().y, -1.0, 1.0) && position.y > start_position.y && !Random(0, 200)) jumped = false;
	}

	if (!hostiled)
	{
		distance = position.x - start_position.x;

		direction = (distance > 100) ? true : (distance < -100) ? false : direction;

		body.applyLinearImpulse((direction ? Vec2{ -VAILLANT_WALK_POWER, 0 } : Vec2{ VAILLANT_WALK_POWER, 0 }) * (240 / Profiler::FPS()));
	}

	state = VaillantState::Idle;

	if (not InRange(body.getVelocity().x, -1.0, 1.0))
	{
		state = VaillantState::Move;

		mirrored = body.getVelocity().x > 0.0;
	}

	mirrored = !mirrored;
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
		}

		break;

	case VaillantState::Death:
	case VaillantState::Destroy:
		assetName = U"Vaillant Death";
		break;
	}

	if (assetName) TextureAsset(assetName)(margin, cutoutSize).mirrored(mirrored).drawAt(position + shiftAmount, mask);

	spriteAnimator.draw();

#ifdef _DEBUG
	body.drawFrame();

	Print << U"Vaillant HP: " << hp << U" / " << max_hp;
	Print << U"Vaillant State: " << static_cast<int>(state);
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
					if (!damaged) player->applyDamage(jumped ? 5 : 10);

					object.getBody().applyLinearImpulse(object.getBody().getPos().x < position.x ? Vec2{ -50, 0 } : Vec2{ 50, 0 });
				}
			}

			SetTimeout([this] { player_hit = false; }, 2000ms);

			player_hit = true;
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
