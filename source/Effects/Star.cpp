#include "Star.h"

Star::Star(Vec2 pos)
{
}

Star::~Star()
{
}

void Star::update()
{
}

void Star::draw() const
{
	//画像の描画
	TextureAsset(U"")(margin, IMG_SIZE, IMG_SIZE).resized(reSize).drawAt(position);
}
