#pragma once
#include "SceneObjectPuppet.h"
#include "BonusPuppeteer.h"
#include "Pointer.h"
#include "PhysicsEngine.h"
#include "SceneObjectPuppeteers.h"

class Bonus : public SceneObjectPuppet<Bonus, BonusPuppeteer>
{
public:
	static Bonus* create(BonusType, PhysicsEngine*);
	
	BonusType GetBonusType() const;

protected:
	Pointer<BonusPuppeteer> CreatePuppeteer(PhysicsEngine*) override;

private:
	void SetBonusType(BonusType);
	BonusType m_bonusType;

	static const float BONUS_SIZE;
};

