# pragma once

# include "../Common.hpp"

enum class AnimationName
{
	None
};

class SpriteAnimator
{
public:
	SpriteAnimator(AnimationName animationName = AnimationName::None, bool looping = false);
	~SpriteAnimator() = default;

	void update();
	void draw() const;

	void setAnimationName(AnimationName animationName) { this->animationName = animationName; }
	void setLooping(bool looping) { this->looping = looping; }

private:
	double frameTime;

	AnimationName animationName;

	bool looping;
};
