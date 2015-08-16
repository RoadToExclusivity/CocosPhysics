#include "CarPuppet.h"
#include "WheelPuppet.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

Car::Car()
	:m_wheel_left(Wheel::create()),
	m_wheel_right(Wheel::create())
{	
}

Car* Car::create(CarSetup &setup)
{
	auto newCar = Car::createFromFile("rect.png");
	newCar->m_cl = setup.GetClearance();
	if (newCar->m_wheel_left && newCar->m_wheel_right)
	{
		newCar->m_wheel_left->SetWheel(setup.GetWheel());
		newCar->m_wheel_right->SetWheel(setup.GetWheel());
	}

	return newCar;
}

void Car::SetEngine(PhysicsEngine* engine)
{
	SceneObjectPuppet < Car, CarPuppeteer >::SetEngine(engine);

	if (m_wheel_left)
	{
		m_wheel_left->SetEngine(engine);
	}
	if (m_wheel_right)
	{
		m_wheel_right->SetEngine(engine);
	}
}

void Car::onEnter()
{
	if (m_wheel_left && m_wheel_right)
	{
		m_wheel_left->setPosition(m_wheel_left->getContentSize().width / 2.0, 
								- m_wheel_left->getContentSize().height / 2.0 * m_wheel_left->getScaleY());

		m_wheel_right->setPosition(getContentSize().width - m_wheel_right->getContentSize().width / 2.0,
								-m_wheel_right->getContentSize().height / 2.0 * m_wheel_right->getScaleY());

		if (m_cl == ClearanceSetup::LOW_CLEARANCE)
		{
			m_wheel_left->setPositionY(m_wheel_left->getPositionY() + 10);
			m_wheel_right->setPositionY(m_wheel_right->getPositionY() + 10);
		}

		this->addChild(m_wheel_left, -1);
		this->addChild(m_wheel_right, -1);		
	}

	SceneObjectPuppet < Car, CarPuppeteer >::onEnter();
}

void Car::ApplyForce(ForceDirection dir)
{
	switch (dir)
	{
	case STOP:
		m_springLeft->EnableMotor(false);

		m_wheel_left->GetBody()->SetLinearDamping(0);
		break;
	case LEFT:
		m_springLeft->EnableMotor(true);
		m_springLeft->SetMotorSpeed(0);

		m_wheel_left->GetBody()->SetLinearDamping(MAX_LINEAR_DAMPING);
		break;
	case RIGHT:
		m_springLeft->EnableMotor(true);
		m_springLeft->SetMotorSpeed(-MAX_MOTOR_SPEED);

		m_wheel_left->GetBody()->SetLinearDamping(0);
		break;
	default:
		break;
	}
}

Pointer<CarPuppeteer> Car::CreatePuppeteer(PhysicsEngine* engine)
{
	b2BodyDef def;
	def.type = b2BodyType::b2_dynamicBody;
	float ptmRatio = engine->getPtmRatio();
	def.position.Set(getPositionX() / ptmRatio, getPositionY() / ptmRatio);

	auto res = CarPuppeteer::create(this, def, engine);
	auto body = res->getBody();
	b2PolygonShape shape;
	shape.SetAsBox(this->getContentSize().width / (2.0 * ptmRatio), this->getContentSize().height / (2.0 * ptmRatio));

	b2FixtureDef fdef;
	fdef.shape = &shape;
	fdef.density = 1.0f;
	fdef.friction = 0.9f;
	fdef.restitution = 0;
	body->CreateFixture(&fdef);	
	
	b2WheelJointDef	wheelDef;
	wheelDef.Initialize(body, m_wheel_left->GetBody(), m_wheel_left->GetBody()->GetPosition(), b2Vec2(0, 1));
	wheelDef.enableMotor = true;
	wheelDef.motorSpeed = 0;
	wheelDef.maxMotorTorque = MAX_MOTOR_TORQUE;
	wheelDef.dampingRatio = 0.7f;
	wheelDef.frequencyHz = m_cl == ClearanceSetup::HIGH_CLEARANCE ? 4.0 : 40.0;

	m_springLeft = (b2WheelJoint*)body->GetWorld()->CreateJoint(&wheelDef);

	wheelDef.Initialize(body, m_wheel_right->GetBody(), m_wheel_right->GetBody()->GetPosition(), b2Vec2(0, 1));
	wheelDef.enableMotor = false;
	wheelDef.motorSpeed = 0;
	wheelDef.maxMotorTorque = MAX_MOTOR_TORQUE;
	wheelDef.dampingRatio = 0.7f;

	m_springRight = (b2WheelJoint*)body->GetWorld()->CreateJoint(&wheelDef);
	//body->SetLinearVelocity(b2Vec2(1, 1));

	return Pointer<CarPuppeteer>(res);
}