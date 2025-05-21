# pragma once

# include "../Objects/ObjectBase.hpp"

class Stage
{
public:
	Stage();
	virtual ~Stage() = default;

	virtual void update();
	virtual void draw() const;

	static Stage* GetInstance();

	bool addObject(ObjectBase* object) { objects << object; return true; }

protected:
	Array<ObjectBase*> objects;

	P2World world;

	const double stepTime;

	double accumulatedTime;

private:
	static Stage* instance;
};
