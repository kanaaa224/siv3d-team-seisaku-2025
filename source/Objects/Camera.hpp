# pragma once

# include "ObjectBase.hpp"

class Camera : public ObjectBase
{
private:
	//Camera2D camera;					//カメラ
	double cameraPosX;					//カメラのX座標
	//double cameraCenterOffset;			//カメラの中心
	//double cameraCenterOffsetVelocity;	//カメラの方向

public:
	Camera(P2World& world, const Vec2& position);
	~Camera();

	void update() override;
	void draw() const override;

	void SetCameraPosX(Vec2 pos);

private:
	void initialize();
	void finalize();
};
