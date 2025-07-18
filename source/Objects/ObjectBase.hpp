/* Copied from "https://github.com/kanaaa224/siv3d-2d-game-base" */

# pragma once

# include "../Common.hpp"

class ObjectBase
{
public:
	ObjectBase(P2World& world, const Vec2& position) { (void)world; (void)position; }
	virtual ~ObjectBase() = default;

	virtual void update() {}     // 更新時に自動で呼び出される関数
	virtual void draw() const {} // 描画時に自動で呼び出される関数

	virtual void onHit(ObjectBase& object, const P2Collision& collision) { (void)object; (void)collision; } // 衝突時に自動で呼び出される関数

	P2Body& getBody() { return body; }

	virtual void destroy() { deleteSelf(); } // 自分自身を破壊する関数

protected:
	P2Body body; // 物理演算のワールドに生成された物体

	void deleteSelf(); // 自分自身のオブジェクト（インスタンス）を削除する関数
};
