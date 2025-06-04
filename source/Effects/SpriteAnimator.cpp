# include "SpriteAnimator.hpp"

SpriteAnimator::SpriteAnimator(AnimationName animationName = AnimationName::None, bool looping = false) : frameTime(0.0), animationName(animationName), looping(looping) {}

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
