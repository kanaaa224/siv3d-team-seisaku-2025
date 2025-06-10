# include "SpriteAnimator.hpp"

SpriteAnimator::SpriteAnimator(AnimationName name) : frameTime(0.0), animationName(name), animationSpeed(0.0), looping(false), mirrored(false), visible(false), state(0), mask({ 1.0, 1.0, 1.0, 1.0 }) {}

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
	case AnimationName::Smoke1: {
		margin = Vec2{ 0, 0 };

		int marginR = 0;

		SizeF textureSize = getTextureSize();

		double frameDuration = animationSpeed ? animationSpeed : 0.085; // 切り替え速度
		int    frameCount    = 12;                                      // 全フレーム数
		int    framesPerRow  = frameCount / 2;                          // 横方向のフレーム数

		int currentFrame = looping ? static_cast<int>(frameTime / frameDuration) % frameCount : std::min(static_cast<int>(frameTime / frameDuration), frameCount - 1);

		int frameX = currentFrame % framesPerRow;
		int frameY = currentFrame / framesPerRow;

		margin.x += (marginR + textureSize.x) * frameX;
		margin.y +=            textureSize.y  * frameY;

		cutoutSize = textureSize;
		break;
	}

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

	case AnimationName::Smoke1:
		assetName = U"Effect 1 29";
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

		region.mirrored(mirrored).drawAt(position, mask);
	}
}

SizeF SpriteAnimator::getTextureSize(AnimationName name) const
{
	if (name == AnimationName::None) name = animationName;

	switch (name)
	{
	case AnimationName::Spark1:
	case AnimationName::Spark2:
	case AnimationName::Smoke1:
		return { 32, 32 };
		break;

	case AnimationName::None:
	default:
		break;
	}

	return { 0, 0 };
}
