# pragma once

# include <Siv3D.hpp>

class ObjectBase {
protected:
	Vec2 position; // 座標
	Vec2 velocity; // 移動速度

	SizeF size; // 大きさ

	Texture texture; // テクスチャ

public:
	ObjectBase(const Vec2& start_position) : position(start_position) {}
	virtual ~ObjectBase() = default;

	virtual void update() {}
	virtual void draw() const {}

	Vec2 getPosition() const { return position; }
	bool setPosition(const Vec2& v) { position  = v; return true; }
	Vec2 getVelocity() const { return velocity; }
	bool addVelocity(const Vec2& v) { velocity += v; return true; }
	SizeF getSize() const { return size; }
	bool drawTexture() const { texture.resized(size).drawAt(position); return true; }
};
