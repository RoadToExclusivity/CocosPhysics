#include "WheelPuppet.h"

USING_NS_CC;

const float Wheel::DENSITY = 1.2f;
const float Wheel::FRICTION = 1;
const float Wheel::RESTITUTION = 0.2f;
const float Wheel::LINEAR_DAMPING = 0.1f;
const float Wheel::BIG_WHEEL_SCALE = 0.7f;
const float Wheel::SMALL_WHEEL_SCALE = 0.6f;

Wheel* Wheel::create()
{
	return Wheel::createFromFile("car_wheel.png");
}

void Wheel::SetWheel(WheelSetup type)
{
	m_wheelType = type;

	if (type == WheelSetup::SMALL_WHEELS)
	{
		setScale(SMALL_WHEEL_SCALE);
	}
	else
	{
		setScale(BIG_WHEEL_SCALE);
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
	shape.m_radius = this->getContentSize().width * this->getScaleX() / (2.0 * ptmRatio);

	b2FixtureDef fdef;
	fdef.shape = &shape;
	fdef.density = DENSITY;
	fdef.friction = FRICTION;
	fdef.restitution = RESTITUTION;
	body->CreateFixture(&fdef);
	body->SetLinearDamping(LINEAR_DAMPING);

	return Pointer<WheelPuppeteer>(res);
}