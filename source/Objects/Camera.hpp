# pragma once

class Camera
{
private:
	Camera2D camera;					//カメラ
	Vec2 cameraPos;						//カメラの座標
	Vec2 playerPos;						//playerの座標
	//double cameraCenterOffset;			//カメラの中心
	//double cameraCenterOffsetVelocity;	//カメラの方向

public:
	Camera();
	~Camera();

	void update();
	void draw() const;

	void SetPlayerPosX(Vec2 pos);
	void SetCameraPosX(Vec2 pos);

private:
	void initialize();
	void finalize();
};
