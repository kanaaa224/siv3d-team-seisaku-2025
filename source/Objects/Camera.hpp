# pragma once

# include "ObjectBase.hpp"

class Camera : public ObjectBase
{
public:
	Camera(P2World& world, const Vec2& position);
	~Camera();

	void update() override;
	void draw() const override;

private:
	void initialize();
	void finalize();
};
