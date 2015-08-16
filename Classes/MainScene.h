#pragma once

#include "cocos2d.h"
#include "PhysicsEngine.h"
#include "CarPuppet.h"
#include "CarSetup.h"

static const int PIXELS_PER_METER = 30;

class MainScene : public cocos2d::Layer
{
public:
	MainScene(CarSetup&);
	static cocos2d::Scene* createScene(CarSetup&);
	static MainScene* create(CarSetup&);
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float) override;
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

private:
	PhysicsEngine m_engine;
	Car* m_car;
	CarSetup m_setup;
};