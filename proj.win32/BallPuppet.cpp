#include "BallPuppet.h"

USING_NS_CC;

Ball* Ball::create()
{
	return Ball::createFromFile("ball.png");
}

Pointer<BallPuppeteer> Ball::CreatePuppeteer(PhysicsEngine* engine)
{
	b2BodyDef def;
	def.type = b2BodyType::b2_dynamicBody;
	def.position = engine->getNodePosition(this);
	auto res = BallPuppeteer::create(this, def, engine);
	auto body = res->getBody();
	b2CircleShape circle;
	circle.m_radius = this->getContentSize().width / (2.0 * engine->getPtmRatio());
	b2FixtureDef fdef;
	fdef.shape = &circle;
	fdef.density = 1000;
	fdef.restitution = 0.6f;
	body->CreateFixture(&fdef);

	return Pointer<BallPuppeteer>(res);
}