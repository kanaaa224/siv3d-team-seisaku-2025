#include "Confetti.h"

void Confetti::launch(int count)
{
	for (int i = 0; i < count; ++i)
	{
		double angle = launchAngle + Random(-launchSpread / 2, launchSpread / 2);
		double speed = Random(2.0, 6.0) * launchPower;
		Vec2 vel = Circular(speed, angle).toVec2();

		ConfettiPara c;
		c.pos = origin;
		c.velocity = vel;
		c.color = HSV{ Random(360.0), 0.6, 1.0 };
		c.width = Random(10.0, 14.0);
		c.height = Random(3.0, 5.0);
		c.zAngle = Random(Math::TwoPi);
		c.angularVelocity = Random(0.05, 0.15) * (RandomBool() ? 1.0 : -1.0);
		confettis << c;
	}
}

void Confetti::update()
{
	for (auto& c : confettis)
	{
		c.update();
	}

	const double sceneHeight = Scene::Height();

	confettis.remove_if([sceneHeight](const ConfettiPara& c)
	{
		return c.pos.y > sceneHeight + 20;
	});
}

void Confetti::draw() const
{
	for (const auto& c : confettis)
	{
		c.draw();
	}
}
