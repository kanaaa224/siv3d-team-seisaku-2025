# pragma once

# include "ObjectBase.hpp"

class Camera : public ObjectBase
{
public:
	Camera(const Vec2& start_position);
	~Camera();

	void update() override;
	void draw() const override;

private:
	void initialize();
	void finalize();
};
