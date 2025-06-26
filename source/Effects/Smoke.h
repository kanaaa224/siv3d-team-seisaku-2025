#pragma once

#include "../Objects/Base.hpp"

class Smoke : public ObjectBase
{
#define IMG_SIZE 64
#define IMG_RESIZE 400
#define IMG_NUM 11
#define CHANGE_IMG 0.1

public:
	Smoke(P2World& world, const Vec2& pos);
	~Smoke();
	void update();
	void draw() const;

private:
	double time_ct;

	Vec2 position;
	Vec2 margin;
	double reSize;
};

