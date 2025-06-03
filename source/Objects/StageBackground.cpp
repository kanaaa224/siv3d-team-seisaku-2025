# include "StageBackground.hpp"

#define SCREEN_WIDTH (1280.0)
#define SCREEN_HEIGHT (720.0)

StageBackground::StageBackground(P2World& world, const Vec2& position) : ObjectBase(world, position)
{
	pos_1st = Vec2(position);						// 0 0
	pos_2nd = Vec2(Scene::Width(), position.y);		// 1280 0
}

void StageBackground::update()
{
	// ステージ1がカメラの左外に出た	Camera.x = 640
	if (pos_1st.x + (SCREEN_WIDTH) < camera_pos.x - 240.0)
	{
		pos_1st.x = pos_2nd.x + SCREEN_WIDTH;
	}

	//ステージ2がカメラの左外に出た
	if (pos_2nd.x + (SCREEN_WIDTH) < camera_pos.x + 180.0)
	{
		pos_2nd.x = pos_1st.x + SCREEN_WIDTH;
	}

	//// ステージ1が右外に出た
	//if (pos_1st.x > cameraTopLeft.x + Scene::Width())
	//{
	//	pos_1st.x = pos_2nd.x - Scene::Width();
	//}

	//// ステージ2が右外に出た
	//if (pos_2nd.x > cameraTopLeft.x + Scene::Width())
	//{
	//	pos_2nd.x = pos_1st.x - Scene::Width();
	//}

}

void StageBackground::draw() const
{
	//最初に描画されている背景 1枚目
	TextureAsset(U"Stage 1 Background").resized(Scene::Size()).draw(pos_1st - (cameraTopLeft) / 2).drawFrame(2, ColorF{Palette::Red});
	//右横に描画されている背景 2枚目
	TextureAsset(U"Stage 1 Background").resized(Scene::Size()).draw(pos_2nd - (cameraTopLeft) / 2).drawFrame(2, ColorF{ Palette::Blue });

	Print << U"Stage1: " << pos_1st;
	Print << U"Stage2: " << pos_2nd;
	Print << U"CameraLeft: " << cameraTopLeft.x;
	Print << U"camera_pos: " << camera_pos;
}

void StageBackground::setCameraPos(Vec2 pos)
{
	camera_pos = pos;
	cameraTopLeft = camera_pos - Vec2(Scene::Width() / 2, Scene::Height() / 2);
	//cameraTopLeft = Vec2(camera_pos.x, camera_pos.y - SCREEN_HEIGHT / 2);
	//cameraTopLeft = Vec2(camera_pos.x + 640.0, camera_pos.y);
}

void StageBackground::setPlayerVelocity(Vec2 velocity)
{
	playerVelocity = velocity;
}
