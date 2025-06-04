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

	void setAnimationName(AnimationName name) { animationName = name; }
	void setAnimationSpeed(double speed) { animationSpeed = speed; }
	void setLooping(bool b) { looping = b; }
	void setPosition(Vec2 p) { position = p; }
	void setSize(SizeF s) { size = s; }

private:
	double frameTime;

	AnimationName animationName;

	double animationSpeed;

	bool looping;

	Vec2 position;

	SizeF size;
};
