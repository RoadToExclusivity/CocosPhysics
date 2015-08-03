#pragma once

#include "NodePhysicsPuppeteer.h"

template <typename Parent, typename T, typename ... Types>
class SceneObjectPuppeteer : public Parent
{
public:
	static T* create(Types ... args)
	{
		T* ret = new T();
		ret->init(args ...);
		ret->autorelease();

		return ret;
	}
};

class BallPuppeteer : public SceneObjectPuppeteer < NodePhysicsPuppeteer, BallPuppeteer, cocos2d::Node*, const b2BodyDef&, PhysicsEngine* >
{
};

class GroundPuppeteer : public SceneObjectPuppeteer < NodePhysicsPuppeteer, GroundPuppeteer, cocos2d::Node*, const b2BodyDef&, PhysicsEngine* >
{
};

class TerrainPuppeteer : public SceneObjectPuppeteer < PhysicsPuppeteer, TerrainPuppeteer, const b2BodyDef&, PhysicsEngine*>
{
public:
	void didUpdatePhysics(){};
	void willUpdatePhysics(float dt){ CC_UNUSED_PARAM(dt); };
};