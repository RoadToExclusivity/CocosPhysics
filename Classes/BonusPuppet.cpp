#include "BonusPuppet.h"

USING_NS_CC;

const float Bonus::BONUS_SIZE = 40;

Bonus* Bonus::create(BonusType type, PhysicsEngine* engine)
{
	Bonus* bonus;

	switch (type)
	{
	case SMALL:
		bonus = Bonus::createFromFile("bonus1.png");
		break;
	case MEDIUM:
		bonus = Bonus::createFromFile("bonus2.png");
		break;
	case BIG:
		bonus = Bonus::createFromFile("bonus3.png");
		break;
	default:
		break;
	}
	
	bonus->setScale(BONUS_SIZE / bonus->getContentSize().width);
	bonus->SetBonusType(type);
	bonus->SetEngine(engine);

	return bonus;
}

void Bonus::SetBonusType(BonusType type)
{
	m_bonusType = type;
}

BonusType Bonus::GetBonusType() const
{
	return m_bonusType;
}

Pointer<BonusPuppeteer> Bonus::CreatePuppeteer(PhysicsEngine* engine)
{
	b2BodyDef def;
	def.type = b2BodyType::b2_staticBody;
	float ptmRatio = engine->getPtmRatio();
	auto pos = getParent()->convertToWorldSpace(getPosition());
	def.position.Set(pos.x / ptmRatio, pos.y / ptmRatio);

	auto res = BonusPuppeteer::create(this, def, engine);
	auto body = res->getBody();
	b2CircleShape shape;
	shape.m_radius = this->getContentSize().height * this->getScaleY() / (2.0 * engine->getPtmRatio());

	b2FixtureDef fdef;
	fdef.shape = &shape;
	fdef.isSensor = true;
	body->CreateFixture(&fdef);

	return Pointer<BonusPuppeteer>(res);
}