#include "BonusPuppeteer.h"
#include "CarPuppeteer.h"
#include "BonusPuppet.h"

USING_NS_CC;

const cocos2d::Vec2 BonusPuppeteer::GRAVITY = Vec2(-10, -250);
const cocos2d::Color4F BonusPuppeteer::START_COLOR_PARTICLE = Color4F(1, 1, 1, 1);
const cocos2d::Color4F BonusPuppeteer::END_COLOR_PARTICLE = Color4F(1, 1, 1, 0);

BonusType BonusPuppeteer::GetBonusType() const
{
	return static_cast<Bonus*>(getNode())->GetBonusType();
}

void BonusPuppeteer::didBeginContact(const PhysicsContactInfo &info)
{
	CarPuppeteer* car = dynamic_cast<CarPuppeteer*>(info.theirPuppeteer);
	if (car)
	{
		auto bonus = (Bonus*)getNode();

		auto fire = ParticleFireworks::create();
		auto parent = bonus->getParent();
		fire->setPosition(bonus->getPositionX(), bonus->getPositionY());
		fire->setDuration(1);
		fire->setLifeVar(0);
		fire->setLife(ACTION_TIME / 2.0);
		
		fire->setStartSize(START_PARTICLE_SIZE);
		fire->setStartSizeVar(0);
		fire->setEndSize(END_PARTICLE_SIZE);
		fire->setEndSizeVar(0);
		fire->setSpeed(PARTICLE_SPEED);
		
		fire->setStartColor(START_COLOR_PARTICLE);
		fire->setStartColorVar(Color4F(0, 0, 0, 0));
		fire->setEndColor(END_COLOR_PARTICLE);
		fire->setEndColorVar(Color4F(0, 0, 0, 0));

		fire->setTotalParticles(PARTICLES);
		fire->setTexture(bonus->getTexture());
		fire->setGravity(GRAVITY);
		fire->setEmissionRate(EMISSION_RATE);
		fire->setAutoRemoveOnFinish(true);
		parent->addChild(fire, 1);

		auto remove = RemoveSelf::create();
		bonus->runAction(remove);
	}
}