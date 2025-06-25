#pragma once

#include "Base.hpp"

#define LEAF_IMG_SIZE 16
#define RESIZE 32

enum eDropPostion
{
	eLeft,
	eCenter,
	eRight
};

class Leaf : public ObjectBase
{
public:
	Leaf(P2World& world, const Vec2& position, int imgNum, eDropPostion dropPos, double windStrength);
	~Leaf();

	void update();
	void draw() const;

private:
	Vec2 margin;
	Vec2 windDirection;//風向き
	double windStr;//風の強さ
};

