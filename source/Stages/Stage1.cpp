# include "Stage1.hpp"

# include "../Objects/GameUI.hpp"
# include "../Objects/Player.hpp"
# include "../Objects/StageBackground.hpp"
# include  "../Objects/Enemy/Scarerun/Scarerun.hpp"

Stage1* Stage1::instance = nullptr;

Stage1::Stage1()
{
	this->initialize();
}

Stage1::~Stage1()
{
	this->finalize();
}

void Stage1::initialize()
{
	objects << new StageBackground(world, Vec2{ 0, 0 });
	objects << new Scarerun(world, Vec2{ 350, 500 });
	objects << new Player(world, Vec2{ 320, 500 });

	floor = world.createRect(P2Static, Vec2{ 640, 600 }, SizeF{ 1000, 10 });
}

void Stage1::update()
{
	for (accumulatedTime += Scene::DeltaTime(); stepTime <= accumulatedTime; accumulatedTime -= stepTime)
	{
		world.update(stepTime);

		for (const auto& object : objects)
		{
			for (auto&& [pair, collision] : world.getCollisions())
			{
				if (pair.a == object->getBody().id())
				{
					for (const auto& objectA : objects)
					{
						if (object->getBody().id() == objectA->getBody().id()) continue;
						if(pair.a != objectA->getBody().id()) continue;

						object->onHit(*objectA);

						break;
					}
				}
				else if (pair.b == object->getBody().id())
				{
					for (const auto& objectB : objects)
					{
						if (object->getBody().id() == objectB->getBody().id()) continue;
						if (pair.a != objectB->getBody().id()) continue;

						object->onHit(*objectB);

						break;
					}
				}
			}
		}
	}

	for (const auto& object : objects)
	{
		object->update();
	}

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->update();
}

void Stage1::draw() const
{
	ClearPrint(); // 過去のPrint出力を消す

	for (const auto& object : objects)
	{
		object->draw();
	}

	floor.draw();

	GameUI* gameUI = GameUI::GetInstance();

	gameUI->draw();
}

void Stage1::finalize()
{
	for (const auto& object : objects)
	{
		delete object;
	}
}

Stage1* Stage1::GetInstance()
{
	if (instance == nullptr) instance = new Stage1();

	return instance;
}
