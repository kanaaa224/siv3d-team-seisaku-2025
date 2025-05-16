# pragma once

# include "../Objects/ObjectBase.hpp"

class StageBackground : public ObjectBase
{
public:
	StageBackground(const Vec2& start_position);
	~StageBackground();

	void update() override;
	void draw() const override;

private:
	void initialize();
	void finalize();
};
