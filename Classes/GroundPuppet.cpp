#include "GroundPuppet.h"

USING_NS_CC;

Ground* Ground::create()
{
	return Ground::createFromFile("groundFone.png");
}

Pointer<GroundPuppeteer> Ground::CreatePuppeteer(PhysicsEngine* engine)
{
	b2BodyDef def;
	def.type = b2BodyType::b2_staticBody;
	float ptmRatio = engine->getPtmRatio();
	def.position.Set(getPositionX() / ptmRatio, getPositionY() / ptmRatio);
	auto res = GroundPuppeteer::create(this, def, engine);
	auto body = res->getBody();
	b2PolygonShape rect;
	rect.SetAsBox(this->getContentSize().width / (2.0 * engine->getPtmRatio()), this->getContentSize().height / (2.0 * engine->getPtmRatio()));
	b2FixtureDef fdef;
	fdef.shape = &rect;
	body->CreateFixture(&fdef);

	return Pointer<GroundPuppeteer>(res);
}