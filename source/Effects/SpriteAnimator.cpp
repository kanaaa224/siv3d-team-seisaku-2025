# include "SpriteAnimator.hpp"

SpriteAnimator::SpriteAnimator(AnimationName name) : frameTime(0.0), animationName(name), animationSpeed(0.0), looping(false), mirrored(false), visible(true), state(0) {}

void SpriteAnimator::update()
{
	if (state) frameTime += Scene::DeltaTime();
}

void SpriteAnimator::draw() const
{
	Vec2 margin;

	SizeF cutoutSize;

	switch (animationName) // アニメーションごとの位置やサイズ決め
	{
	case AnimationName::Spark1:
	case AnimationName::Spark2:
		break;

	case AnimationName::None:
	default:
		break;
	}

	String assetName;

	switch (animationName) // アニメーションごとのアセット名
	{
	case AnimationName::Spark1:
		assetName = U"Effect 1 1";
		break;

	case AnimationName::Spark2:
		assetName = U"Effect 1 2";
		break;

	case AnimationName::None:
	default:
		break;
	}

	if (assetName && visible)
	{
		Texture       texture = TextureAsset(assetName);
		TextureRegion region  = texture(margin, cutoutSize);

		if (size != SizeF{ 0, 0 }) region = region.resized(size);

		region.mirrored(mirrored).drawAt(position);
	}
}
