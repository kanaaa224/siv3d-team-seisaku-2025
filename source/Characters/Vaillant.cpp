# include "Vaillant.hpp"
# include "Player.hpp"
# include "../Stage.hpp"
# include "../Objects/HitBox.hpp"

Vaillant::Vaillant(P2World& world, const Vec2& position) :
	CharacterBase(world, position),
	start_position(position),
	frameTime(0.0),
	attack_frame(0.0),
	state(VaillantState::Idle),
	jumped(false),
	roaming_flipped(false),
	mirrored(false),
	attack_started(false),
	die_executed(false),
	destroy_executed(false),
	damaged(false),
	attacking(false)
{
	body = world.createRect(P2Dynamic, position, size = { VAILLANT_SIZE }, { .density = 0.0 }, { .categoryBits = CollisionCategory::Enemy, .maskBits = CollisionCategory::All & ~CollisionCategory::Player });

	body.addRectSensor(RectF{ -(size / 2), size }, { .categoryBits = CollisionCategory::Enemy, .maskBits = CollisionCategory::Player });

	body.setFixedRotation(true);

	initialize();
}

void Vaillant::initialize()
{
	max_hp = VAILLANT_MAX_HP;

	hp = max_hp;
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
	if (KeyJ.pressed()) body.applyLinearImpulse({ -10,     0 });
	if (KeyI.down())    body.applyLinearImpulse({   0, -2500 });
	if (KeyL.pressed()) body.applyLinearImpulse({  10,     0 });

	if (KeyH.down()) heal(10);
	if (KeyP.down()) applyDamage(10);
#endif

	spriteAnimator.update();

	if (state >= VaillantState::Death) return;

	double distance = position.distanceFrom(player_position);

	if (distance <= 400) attack_started = true;

	if (attack_started)
	{
		if (state == VaillantState::Attack)
		{
			if (!attacking)
			{
				attack_frame = frameTime;

				std::thread([this]()
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));

					AudioAsset(U"Vaillant Attack").playOneShot();
				}).detach();

				std::thread([this]()
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1500));

					Stage::GetInstance()->createObject<HitBox>(position + Vec2{ (mirrored ? -size.x / 2 - 200 : size.x / 2), 0 }, *this);
				}).detach();

				std::thread([this]()
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(2000));

					state = VaillantState::Idle;

					attacking = false;
				}).detach();

				attacking = true;
			}

			return;
		}

		bool walking_direction = false;

		distance = position.x - player_position.x;

		if (distance >  10) walking_direction = true;
		if (distance < -10) walking_direction = false;

		body.applyLinearImpulse((walking_direction ? Vec2{ -VAILLANT_WALK_POWER, 0 } : Vec2{ VAILLANT_WALK_POWER, 0 }) * (240 / Profiler::FPS()));

		distance = Abs(distance);

		if (!jumped && distance <= 300)
		{
			if (Random(0, 1))
			{
				state = VaillantState::Attack;

				mirrored = player_position.x <= position.x ? true : false;

				return;
			}
			
			AudioAsset(U"Vaillant Jump").playOneShot();

			body.applyLinearImpulse(Vec2{ 0, -VAILLANT_JUMP_POWER });

			spriteAnimator.setAnimationName(AnimationName::Smoke1);
			spriteAnimator.setMask({ 1.0, 1.0, 1.0, 0.5 });
			spriteAnimator.setSize({ 200, 150 });
			spriteAnimator.setPosition({ position.x, (position.y + (size.y / 2)) });
			spriteAnimator.stop();
			spriteAnimator.show();
			spriteAnimator.play();

			jumped = true;
		}

		if (InRange(body.getVelocity().y, -1.0, 1.0) && position.y > start_position.y && !Random(0, 200)) jumped = false;
	}
	else
	{
		distance = position.x - start_position.x;

		if (distance >  100) roaming_flipped = true;
		if (distance < -100) roaming_flipped = false;

		body.applyLinearImpulse((roaming_flipped ? Vec2{ -VAILLANT_WALK_POWER, 0 } : Vec2{ VAILLANT_WALK_POWER, 0 }) * (240 / Profiler::FPS()));
	}

	state = VaillantState::Idle;

	if (not InRange(body.getVelocity().x, -1.0, 1.0)) state = VaillantState::Walk;
	if (not InRange(body.getVelocity().x, -1.0, 1.0)) mirrored = body.getVelocity().x > 0.0;

	mirrored = !mirrored;
}

void Vaillant::draw() const
{
	Vec2 margin;

	SizeF cutoutSize { 255, 233 };

	double frameDuration = 0.0;
	int    frameCount    = 0;

	static int currentFrame = 0;

	ColorF mask { 1.0, 1.0, 1.0, 1.0 };
	
	Vec2 shiftAmount { 0, -16 };

	switch (state)
	{
	case VaillantState::Idle:
	case VaillantState::Walk:
	{
		margin = { 86, 81 };

		int marginR = 195;

		frameDuration = 0.085;
		frameCount    = 16;

		currentFrame = static_cast<int>(frameTime / frameDuration) % frameCount;

		if (not InRange(body.getVelocity().y, -1.0, 1.0)) currentFrame = 12;

		if (currentFrame) margin.x += (marginR + cutoutSize.x) * currentFrame;

		break;
	}

	case VaillantState::Attack:
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

	case VaillantState::Death:
	case VaillantState::Destroy:
	{
		margin = { 86, 81 };

		int marginR = 195;

		frameDuration = 0.15;
		frameCount    = 16;
		
		static bool initialized = false;
		
		if (!initialized)
		{
			currentFrame = 0;
			
			initialized = true;
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
	}
	}

	String assetName;

	switch (state)
	{
	case VaillantState::Idle:
		assetName = U"Vaillant Idle";
		break;

	case VaillantState::Walk:
		assetName = U"Vaillant Walk";
		break;

	case VaillantState::Attack:
		assetName = U"Vaillant Attack";
		break;

	case VaillantState::Death:
	case VaillantState::Destroy:
		assetName = U"Vaillant Death";
		break;
	}

	TextureAsset(assetName)(margin, cutoutSize).mirrored(mirrored).drawAt(position + shiftAmount, mask);

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
		if (player->getplayerstate() != ePlayerState::attack &&
			player->getplayerstate() != ePlayerState::avoidance &&
			player->getplayerstate() != ePlayerState::jump_attack &&
			player->getplayerstate() != ePlayerState::jump_avoidance)
		{
			if (state <= VaillantState::Attack) player->applyDamage(jumped ? 5 : 20);
		}
	}
}

void Vaillant::destroy()
{
	state = VaillantState::Destroy;

	if (!destroy_executed)
	{
		frameTime = 0.0;

		body.release();

		std::thread([this]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			ObjectBase::destroy();
		}).detach();

		destroy_executed = true;
	}
}

void Vaillant::die()
{
	state = VaillantState::Death;

	if (!die_executed)
	{
		AudioAsset(U"Vaillant Death").playOneShot();

		std::thread([this]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));

			CharacterBase::die();
		}).detach();

		die_executed = true;
	}
}

void Vaillant::onDamaged(float amount)
{
	if (!damaged)
	{
		CharacterBase::onDamaged(amount);

		AudioAsset(U"Vaillant Damage").playOneShot();

		std::thread([this]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(750));

			damaged = false;
		}).detach();

		damaged = true;
	}
}
