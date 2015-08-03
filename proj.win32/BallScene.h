#pragma once

#include "cocos2d.h"
#include "PhysicsEngine.h"

static const int PIXELS_PER_METER = 30;

class BallScene : public cocos2d::Layer
{
public:
	BallScene();
	static cocos2d::Scene* createScene();
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float) override;

	CREATE_FUNC(BallScene);

private:
	PhysicsEngine m_engine;
};