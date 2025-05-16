# pragma once

# include "ObjectBase.hpp"

class Stage
{
public:
	Stage();
	~Stage();

	void update();
	void draw() const;

	static Stage* GetInstance();

private:
	static Stage* instance;

	Array<ObjectBase*> objects;

	void initialize();
	void finalize();
};
