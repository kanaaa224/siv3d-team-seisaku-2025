# include "HitBox.hpp"
# include "../Objects/Enemy/EnemyBase.hpp"
# include "../Stages/Stage.hpp"
# include "../Objects/Player.hpp"

bool HitBox::flg = false;

HitBox::HitBox(P2World& world, const Vec2& position) : ObjectBase(world, position)
{
	body = world.createRectSensor(P2Kinematic, position, RectF{ 20, 90 });

	this->initialize();
}

HitBox::~HitBox()
{
	this->finalize();
}

void HitBox::initialize()
{
	if (flg == true)
	{
		Stage::GetInstance()->deleteObject(this);
	}

	flg = true;
}

void HitBox::update()
{
	if (flip_flg == false)
	{
		body.setPos(Vec2(playerPos.x + 28, playerPos.y - 45));
	}
	else if(flip_flg == true)
	{
		body.setPos(Vec2(playerPos.x - 48, playerPos.y - 45));
	}


	if (playerState != ePlayerState::attack)
	{
		Stage::GetInstance()->deleteObject(this);
		flg = false;
	}
}

void HitBox::draw() const
{
	body.drawFrame(1.0, ColorF(Palette::Red));
}

void HitBox::finalize()
{

}

void HitBox::onHit(ObjectBase& object)
{
	if (EnemyBase* enemy = dynamic_cast<EnemyBase*>(&object))
	{
		enemy->onDamaged(50);
		Stage::GetInstance()->deleteObject(this);
		flg = false;
	}
}
