# include "Camera.hpp"

Camera::Camera() :
	playerPos(0.0, 0.0),
	cameraPos(0.0, 0.0),
	camera(cameraPos, 1.0, CameraControl::None_)
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
	cameraPos = camera.getCenter();
	camera.setTargetCenter(Vec2{playerPos.x, 360});
	camera.update();
}

void Camera::draw() const
{

}

void Camera::SetPlayerPosX(Vec2 pos)
{
	playerPos = pos;
}

void Camera::SetCameraPosX(Vec2 pos)
{
	cameraPos = pos;
}

void Camera::finalize()
{

}
