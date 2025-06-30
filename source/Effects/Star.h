#pragma once

#include "../Objects/ObjectBase.hpp"

class Star : public ObjectBase
{
#define IMG_NUM 4
#define IMG_SIZE 32
#define CHANGE_IMG 0.1

public:
	Star(P2World& world, const Vec2& pos);
	~Star();

	void update();
	void draw() const;

private:
	double time_ct;

	Vec2 position;
	Vec2 margin;
	double reSize;
};

