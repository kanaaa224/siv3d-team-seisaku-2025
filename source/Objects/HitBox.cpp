# include "HitBox.hpp"
# include "../Objects/Enemy/EnemyBase.hpp"

HitBox::HitBox(P2World& world, const Vec2& position) : ObjectBase(world, position)
{
	body = world.createRectSensor(P2Kinematic, position, RectF{ 70, 90 });

	this->initialize();
}

HitBox::~HitBox()
{
	this->finalize();
}

void HitBox::initialize()
{
	
}

void HitBox::update()
{

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
		
	}
}
