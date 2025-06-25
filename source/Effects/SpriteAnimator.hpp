# pragma once

# include "../Common.hpp"

enum class AnimationName
{
	None,
	Spark1,
	Spark2,
	Smoke1,
	Smoke2,
	SpawnEffect
};

class SpriteAnimator
{
public:
	SpriteAnimator(AnimationName name = AnimationName::None);
	~SpriteAnimator() = default;

	void update();
	void draw() const;

	void setAnimationName(AnimationName n) { animationName = n; } // アニメーション名を設定
	void setAnimationSpeed(double s) { animationSpeed = s; }      // アニメーションの速度を設定
	void setLooping(bool b) { looping = b; }                      // ループ再生するかどうかを設定
	void setMirrored(bool b) { mirrored = b; }                    // 反転するかどうかを設定
	void setPosition(Vec2 p) { position = p; }                    // 位置を設定
	void setSize(SizeF s) { size = s; }                           // 大きさを設定
	void setMask(ColorF c) { mask = c; }                          // マスクカラーを設定

	int   getState() const { return state; }
	SizeF getTextureSize(AnimationName name = AnimationName::None) const;

	void play () { state = 1; }; // 再生
	void pause() { state = 0; }; // 一時停止

	void stop() { state = 0; frameTime = 0.0; }; // 停止

	void show() { visible = true;  }; // 表示する
	void hide() { visible = false; }; // 隠す

private:
	double frameTime, animationSpeed;

	AnimationName animationName;

	bool looping, mirrored, visible;

	int state;

	Vec2 position;

	SizeF size;

	ColorF mask;
};
