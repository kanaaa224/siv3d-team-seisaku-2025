#pragma once

#include "ObjectBase.hpp"

#define LEAF_IMG_SIZE 16

#define RESIZE_MIN 20
#define RESIZE_MAX 40

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

	double reSize;

	double lifeTime;

	double timeOffset;

	Vec2 spawnPos;

private:
	void movement();
};

