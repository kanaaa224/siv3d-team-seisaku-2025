# include "Camera.hpp"

Camera::Camera(P2World& world, const Vec2& position): ObjectBase(world, position)
{
	this->initialize();
}

Camera::~Camera()
{
	this->finalize();
}

void Camera::initialize()
{

}

void Camera::update()
{

}

void Camera::draw() const
{

}

void Camera::SetCameraPosX(Vec2 pos)
{
	cameraPosX = pos.x;
}

void Camera::finalize()
{

}
