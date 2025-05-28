# pragma once

# include "../Objects/Base.hpp"

class Stage
{
public:
	Stage();
	virtual ~Stage();

	virtual void update();     // 更新時に自動で呼び出される関数
	virtual void draw() const; // 描画時に自動で呼び出される関数

	template <class T, class... Args>
	void createObject(Args&&... args) { static_assert(std::is_base_of<ObjectBase, T>::value); objects << new T(world, std::forward<Args>(args)...); } // ステージ上にオブジェクトを生成する関数
	void deleteObject(ObjectBase* object) { if (object && !deletionObjects.contains(object)) deletionObjects << object; }                             // ステージ上のオブジェクトを削除する関数

	static void   NewInstance(); // 新規インスタンスを生成する関数
	static Stage* GetInstance(); // 既存のインスタンスを返す関数

protected:
	Array<ObjectBase*> objects;         // ステージ上に存在するオブジェクト
	Array<ObjectBase*> deletionObjects; // 削除予定のオブジェクト

	P2World world; // 物理演算用

	const double stepTime; // 物理演算用

	double accumulatedTime; // 物理演算用

	static Stage* instance;
};
