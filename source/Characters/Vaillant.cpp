# include "Vaillant.hpp"
# include "Player.hpp"

Vaillant::Vaillant(P2World& world, const Vec2& position) :
	CharacterBase(world, position),
	start_position(position),
	frameTime(0.0),
	state(VaillantState::Idle),
	mirrored(false),
	attack_started(false),
	die_executed(false),
	destroy_executed(false),
	damaged(false)
{
	body = world.createRect(P2Dynamic, position, size = { 203, 233 });

	body.setFixedRotation(true);

	initialize();
}

void Vaillant::initialize()
{
	max_hp = 100;

	hp = max_hp;
}

void Vaillant::update()
{
	frameTime += Scene::DeltaTime();

	if (body) position = body.getPos();

	if (position.y >= 1000)
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

	{
		spriteAnimator.setAnimationName(AnimationName::Spark1);
		spriteAnimator.setSize({ 100, 100 });

		if (KeyZ.down())
		{
			spriteAnimator.show();
			spriteAnimator.play();
		}

		if (KeyX.down())
		{
			spriteAnimator.hide();
			spriteAnimator.stop();
		}

		spriteAnimator.setPosition(position);
		spriteAnimator.update();
	}
#endif

	if (state >= VaillantState::Death) return;

	double distance = position.distanceFrom(player_position);

	if (distance <= 400) attack_started = true;

	if (attack_started)
	{
		static bool jumped = false;

		bool walking_direction = false;

		distance = position.x - player_position.x;

		if (distance >  100) walking_direction = true;
		if (distance < -100) walking_direction = false;

		body.applyLinearImpulse((walking_direction ? Vec2{ -10, 0 } : Vec2{ 10, 0 }) * (240 / Profiler::FPS()));

		distance = Abs(distance);

		if (!jumped && distance <= 300)
		{
			body.applyLinearImpulse(Vec2{ 0, -2500 });

			jumped = true;
		}

		if (distance >= 400) jumped = false;
	}
	else
	{
		static bool roaming_flipped = false;

		distance = position.x - start_position.x;

		if (distance >  100) roaming_flipped = true;
		if (distance < -100) roaming_flipped = false;

		body.applyLinearImpulse((roaming_flipped ? Vec2{ -10, 0 } : Vec2{ 10, 0 }) * (240 / Profiler::FPS()));
	}

	state = VaillantState::Idle;

	if (not InRange(body.getVelocity().x, -1.0, 1.0)) state = VaillantState::Walk;
	if (not InRange(body.getVelocity().x, -1.0, 1.0)) mirrored = body.getVelocity().x > 0.0;

	switch (state)
	{
	case VaillantState::Idle:
	case VaillantState::Walk:
		mirrored = !mirrored;
		break;
	}
}

void Vaillant::draw() const
{
	Vec2 margin;

	SizeF cutoutSize = size;

	double frameDuration = 0.0;
	int    frameCount    = 0;

	static int currentFrame = 0;

	ColorF mask = { 1.0, 1.0, 1.0, 1.0 };

	switch (state)
	{
	case VaillantState::Idle:
	case VaillantState::Walk:
	{
		margin = { 112, 81 };

		int marginR = 247;

		frameDuration = 0.085;
		frameCount    = 16;

		currentFrame = static_cast<int>(frameTime / frameDuration) % frameCount;

		if (not InRange(body.getVelocity().y, -1.0, 1.0)) currentFrame = 12;

		if (currentFrame) margin.x += (marginR + cutoutSize.x) * currentFrame;

		break;
	}

	case VaillantState::Death:
	case VaillantState::Destroy:
	{
		margin = { 112, 81 };

		int marginR = 247;

		frameDuration = 0.085;
		frameCount    = 16;

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

	case VaillantState::Death:
	case VaillantState::Destroy:
		assetName = U"Vaillant Death";
		break;
	}

	TextureAsset(assetName)(margin, cutoutSize).mirrored(mirrored).drawAt(position, mask);

#ifdef _DEBUG
	body.drawFrame();

	Print << U"Vaillant HP: " << hp << U" / " << max_hp;
	Print << U"Vaillant State: " << static_cast<int>(state);

	spriteAnimator.draw();
#endif
}

void Vaillant::onHit(ObjectBase& object)
{
	if (Player* player = dynamic_cast<Player*>(&object))
	{
		if (object.getBody().getPos().y < (position.y - 100))
		{
			object.getBody().applyLinearImpulse(Vec2{ 0, -40 });

			this->applyDamage(10);
		}
		else
		{
			if (object.getBody().getPos().x < position.x)
			{
				object.getBody().applyLinearImpulse(Vec2{ -10, -10 });
			}
			else
			{
				object.getBody().applyLinearImpulse(Vec2{ 10, -10 });
			}

			player->applyDamage(10);
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
	CharacterBase::onDamaged(amount);

	if (!damaged)
	{
		std::thread([this]()
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(250));

			damaged = false;
		}).detach();

		damaged = true;
	}
}
