#pragma once
class Star
{
#define IMG_SIZE 32

public:
	Star(Vec2 pos);
	~Star();

	void update();
	void draw() const;

private:
	Vec2 position;
	Vec2 margin;
	double reSize;
};

