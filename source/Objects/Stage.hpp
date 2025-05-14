#pragma once

#include "../Objects/ObjectBase.hpp"

class Stage : public ObjectBase
{
private:


public:
	Stage(const Vec2& start_position);
	~Stage();

	void initialize();
	void update() override;
	void draw() const override;
	void finalize();

};
