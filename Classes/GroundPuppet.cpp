#include "GroundPuppet.h"

USING_NS_CC;

Ground* Ground::create(PhysicsEngine* engine)
{
	auto ground = Ground::createFromFile("groundFone.png");
	ground->SetEngine(engine);

	return ground;
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
	rect.SetAsBox(this->getContentSize().width / (2.0 * ptmRatio), this->getContentSize().height / (2.0 * ptmRatio));
	b2FixtureDef fdef;
	fdef.shape = &rect;
	body->CreateFixture(&fdef);

	return Pointer<GroundPuppeteer>(res);
}