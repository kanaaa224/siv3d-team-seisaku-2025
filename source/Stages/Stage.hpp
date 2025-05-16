﻿# pragma once

# include "../Objects/ObjectBase.hpp"

class Stage
{
public:
	Stage();
	virtual ~Stage() = default;

	virtual void update();
	virtual void draw() const;

	static Stage* GetInstance();

protected:
	Array<ObjectBase*> objects;

private:
	static Stage* instance;
};
