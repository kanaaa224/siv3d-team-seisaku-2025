# include "SpriteAnimator.hpp"

SpriteAnimator::SpriteAnimator(AnimationName animationName, bool looping) : frameTime(0.0), animationName(animationName), animationSpeed(0.0), looping(looping) {}

void SpriteAnimator::update()
{
	frameTime += Scene::DeltaTime();
}

void SpriteAnimator::draw() const
{
	switch (animationName)
	{
	case AnimationName::None:
	default:
		break;
	}

#ifdef _DEBUG

#endif
}
