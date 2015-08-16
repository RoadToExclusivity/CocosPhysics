#include "WheelPuppet.h"

USING_NS_CC;

Wheel* Wheel::create()
{
	return Wheel::createFromFile("wheel-48.png");
}

void Wheel::SetWheel(WheelSetup type)
{
	m_wheelType = type;

	if (type == WheelSetup::SMALL_WHEELS)
	{
		setScale(0.75, 0.75);
	}
	else
	{
		setScale(1, 1);
	}
}

Pointer<WheelPuppeteer> Wheel::CreatePuppeteer(PhysicsEngine* engine)
{
	b2BodyDef def;
	def.type = b2BodyType::b2_dynamicBody;
	float ptmRatio = engine->getPtmRatio();
	auto pos = getParent()->convertToWorldSpace(getPosition());
	def.position.Set(pos.x / ptmRatio, pos.y / ptmRatio);

	auto res = WheelPuppeteer::create(this, def, engine);
	auto body = res->getBody();
	b2CircleShape shape;
	shape.m_radius = this->getContentSize().width * this->getScaleX() / (2.0 * engine->getPtmRatio());

	b2FixtureDef fdef;
	fdef.shape = &shape;
	fdef.density = 1.5f;
	fdef.friction = 0.9f;
	fdef.restitution = 0.2f;
	body->CreateFixture(&fdef);

	return Pointer<WheelPuppeteer>(res);
}