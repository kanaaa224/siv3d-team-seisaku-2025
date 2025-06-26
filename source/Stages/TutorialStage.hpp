# pragma once

# include "../Stage.hpp"

class TutorialStage : public Stage
{
public:
	TutorialStage();
	~TutorialStage() = default;

	void update() override;
	void draw() const override;

	static void NewInstance();

private:
	void initialize();
};

