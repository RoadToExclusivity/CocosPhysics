#pragma once
#include "cocos2d.h"
#include "SceneObjectPuppeteers.h"
#include "NodePhysicsPuppeteer.h"
#include "PhysicsEngine.h"

class CarPuppeteer : public SceneObjectPuppeteer < NodePhysicsPuppeteer, CarPuppeteer, cocos2d::Node*, const b2BodyDef&, PhysicsEngine* >
{
public:
	void didBeginContact(const PhysicsContactInfo &info) override;
};
