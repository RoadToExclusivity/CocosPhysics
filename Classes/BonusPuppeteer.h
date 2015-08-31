#pragma once
#include "cocos2d.h"
#include "PhysicsEngine.h"
#include "PhysicsPuppeteer.h"
#include "SceneObjectPuppeteers.h"
#include "NodePhysicsPuppeteer.h"

enum BonusType
{
	SMALL,
	MEDIUM,
	BIG
};

class BonusPuppeteer : public SceneObjectPuppeteer < NodePhysicsPuppeteer, BonusPuppeteer, cocos2d::Node*, const b2BodyDef&, PhysicsEngine* >
{
public:
	void didBeginContact(const PhysicsContactInfo &info) override;
	BonusType GetBonusType() const;

private:
	static const int PARTICLES = 3;
	static const int START_PARTICLE_SIZE = 37;
	static const int END_PARTICLE_SIZE = 20;
	static const int PARTICLE_SPEED = 50;
	static const int ACTION_TIME = 6;
	static const int EMISSION_RATE = 50;
	static const cocos2d::Vec2 GRAVITY;
	static const cocos2d::Color4F START_COLOR_PARTICLE;
	static const cocos2d::Color4F END_COLOR_PARTICLE;
};

