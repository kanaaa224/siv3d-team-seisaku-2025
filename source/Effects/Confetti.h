#pragma once

#include <Siv3D.hpp>

struct ConfettiPara
{
	Vec2 pos;
	Vec2 velocity;
	ColorF color;
	double width, height;
	double zAngle;
	double angularVelocity;

	void update()
	{
		velocity.y += 0.05;
		velocity.x *= 0.99;
		pos += velocity;
		zAngle += angularVelocity;
	}

	void draw() const
	{
		double scaleX = std::cos(zAngle);
		double visualWidth = width * std::abs(scaleX);
		double alpha = 0.5 + 0.5 * std::abs(scaleX);
		RectF rect{ Arg::center(pos), visualWidth, height };
		rect.draw(color.withAlpha(alpha));
	}
};

class Confetti
{
private:
	Array<ConfettiPara> confettis;

public:
	Vec2 origin = Scene::Center();
	double launchAngle = -Math::HalfPi;
	double launchSpread = Math::Pi / 3;
	double launchPower = 1.0;

	// コンストラクタ
	Confetti(const Vec2& origin_, double launchPower_ = 1.0, double launchAngle_ = -Math::HalfPi)
		: origin(origin_)
		, launchPower(launchPower_)
		, launchAngle(launchAngle_)
	{
	}

	void launch(int count = 600);

	void update();
	void draw() const;
};

