# include "Camera.hpp"

Camera::Camera(const Vec2& start_position): ObjectBase(start_position)
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
	TextureAsset(U"Stage 1 Background").resized(Scene::Size()).draw(position); // 仮
}

void Camera::finalize()
{

}
