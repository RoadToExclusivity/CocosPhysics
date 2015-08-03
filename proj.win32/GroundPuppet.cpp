#include "GroundPuppet.h"

USING_NS_CC;

Ground* Ground::create()
{
	return Ground::createFromFile("ground.jpg");
}

Pointer<GroundPuppeteer> Ground::CreatePuppeteer(PhysicsEngine* engine)
{
	b2BodyDef def;
	def.type = b2BodyType::b2_staticBody;
	def.position = engine->getNodePosition(this);
	auto res = GroundPuppeteer::create(this, def, engine);
	auto body = res->getBody();
	b2PolygonShape rect;
	rect.SetAsBox(this->getContentSize().width / (2.0 * engine->getPtmRatio()), this->getContentSize().height / (2.0* engine->getPtmRatio()));
	b2FixtureDef fdef;
	fdef.shape = &rect;
	body->CreateFixture(&fdef);

	return Pointer<GroundPuppeteer>(res);
}