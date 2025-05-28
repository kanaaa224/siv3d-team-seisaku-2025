# pragma once

# include <Siv3D.hpp>

class ObjectBase
{
protected:
	P2Body body; // 物理演算のワールドに生成された物体

public:
	ObjectBase(P2World& world, const Vec2& position) { (void)world; (void)position; }
	virtual ~ObjectBase() = default;

	virtual void update() {}     // 更新時に自動で呼び出される関数
	virtual void draw() const {} // 描画時に自動で呼び出される関数

	virtual void onHit(ObjectBase& object) { (void)object; } // 衝突時に自動で呼び出される関数

	P2Body& getBody() { return body; }
};
