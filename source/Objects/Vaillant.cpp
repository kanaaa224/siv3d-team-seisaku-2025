# include "Vaillant.hpp"
# include "Player.hpp"

Vaillant::Vaillant(P2World& world, const Vec2& position) : CharacterBase(world, position), start_position(position), animationTime(0.0), state(0), mirrored(false)
{
	body = world.createRect(P2Dynamic, position, size = SizeF{ 203, 233 });

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
	if (body.getPos().y >= 1000)
	{
		body.setPos(start_position);
		body.setVelocity(Vec2{});
	}
	
#ifdef _DEBUG
	if (KeyJ.pressed()) body.applyLinearImpulse(Vec2{ -10,     0 });
	if (KeyI.down())    body.applyLinearImpulse(Vec2{   0, -2500 });
	if (KeyL.pressed()) body.applyLinearImpulse(Vec2{  10,     0 });
	
	if (KeyH.down()) heal(10);
#endif
	
	double distance = body.getPos().distanceFrom(player_position);
	
	if (distance <= 500) attack_started = true;
	
	if(attack_started)
	{
		static bool jumped = false;

		bool walking_direction = false;
		
		distance = body.getPos().x - player_position.x;
		
		if (distance >  100) walking_direction = true;
		if (distance < -100) walking_direction = false;
		
		body.applyLinearImpulse(walking_direction ? Vec2{ -10, 0 } : Vec2{ 10, 0 });
		
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

		distance = body.getPos().x - start_position.x;
		
		if (distance >  100) roaming_flipped = true;
		if (distance < -100) roaming_flipped = false;

		body.applyLinearImpulse(roaming_flipped ? Vec2{ -8.5, 0 } : Vec2{ 8.5, 0 });
	}
	
	state = 0;
	
	if (not InRange(body.getVelocity().x, -1.0, 1.0)) state = 1;
	if (not InRange(body.getVelocity().x, -1.0, 1.0)) mirrored = body.getVelocity().x > 0.0;
	
	switch(state)
	{
		case 0:
		case 1:
			mirrored = !mirrored;
			break;
	}
	
	animationTime += Scene::DeltaTime();
}

void Vaillant::draw() const
{
	Vec2 margin;
	
	double frameDuration = 0.0;
	int    frameCount    = 0;
	
	int currentFrame = 0;

	switch(state)
	{
		case 0:
		case 1:
			margin = Vec2{ 112, 81 };
			
			int marginR = 247;
			
			frameDuration = 0.085;
			frameCount    = 16;
			
			currentFrame = static_cast<int>(animationTime / frameDuration) % frameCount;
			
			if (not InRange(body.getVelocity().y, -1.0, 1.0)) currentFrame = 12;
			
			if (currentFrame) margin.x += (marginR + size.x) * currentFrame;

			break;
	}
	
	String assetName;
	
	switch(state)
	{
		case 0:
			assetName = U"Vaillant Idle";
			break;
			
		case 1:
			assetName = U"Vaillant Walk";
			break;
	}

	TextureAsset(assetName)( margin, size ).mirrored(mirrored).drawAt(body.getPos());

#ifdef _DEBUG
	body.drawFrame();
	
	Print << U"Vaillant HP: " << hp << U" / " << max_hp;
#endif
}

void Vaillant::onHit(ObjectBase& object)
{
	if (Player* player = dynamic_cast<Player*>(&object))
	{
		if (object.getBody().getPos().y < (body.getPos().y - 100))
		{
			object.getBody().applyLinearImpulse(Vec2{ 0, -100 });

			this->applyDamage(10);
		}
		else
		{
			if (object.getBody().getPos().x < body.getPos().x)
			{
				object.getBody().applyLinearImpulse(Vec2{ -50, -50 });
			}
			else
			{
				object.getBody().applyLinearImpulse(Vec2{ 50, -50 });
			}

			player->applyDamage(10);
		}
	}
}
