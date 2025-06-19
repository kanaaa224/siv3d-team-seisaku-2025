#include "BuffSpawnEffect.h"

BuffSpawnEffect::BuffSpawnEffect(P2World& world, const Vec2& postion, ObjectBase& ownerClass)
	: ObjectBase(world, postion),
	  owner(&ownerClass)
{
	BuffSpawnEffect::owner = &ownerClass;

	assetName = U"";
	margin = Vec2{ 0.0,0.0 };
	reSize = Vec2{ IMG_RESIZE,IMG_RESIZE };
	pos = postion;
	imgChange_ct = 0.0;
}

BuffSpawnEffect::~BuffSpawnEffect()
{
}

void BuffSpawnEffect::update()
{
}

void BuffSpawnEffect::draw() const
{
}
