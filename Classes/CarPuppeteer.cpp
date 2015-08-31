#include "CarPuppeteer.h"
#include "BonusPuppeteer.h"
#include "SceneObjectPuppeteers.h"
#include "BonusPuppet.h"
#include "CarPuppet.h"

USING_NS_CC;

void CarPuppeteer::didBeginContact(const PhysicsContactInfo &info)
{
	BonusPuppeteer* bonus = dynamic_cast<BonusPuppeteer*>(info.theirPuppeteer);
	TerrainPuppeteer* terrain = dynamic_cast<TerrainPuppeteer*>(info.theirPuppeteer);
	auto car = static_cast<Car*>(getNode());
	
	if (bonus)
	{
		auto bonusType = bonus->GetBonusType();
		unsigned int typeCost = 0;
		switch (bonusType)
		{
		case SMALL:
			typeCost = 50;
			break;
		case MEDIUM:
			typeCost = 100;
			break;
		case BIG:
			typeCost = 150;
			break;
		default:
			break;
		}

		car->AddPoints(typeCost);
	}

	if (terrain)
	{
		car->SetContactWithTerrain(true);
	}
	else
	{
		car->SetContactWithTerrain(false);
	}
}