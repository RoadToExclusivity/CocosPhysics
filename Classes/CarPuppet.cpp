#include "CarPuppet.h"
#include "WheelPuppet.h"
#include "Box2D\Box2D.h"

USING_NS_CC;

Car::Car()
	:m_wheel_left(Wheel::create()),
	m_wheel_right(Wheel::create())
{	
}

Car* Car::create(CarSetup &setup, PhysicsEngine* engine)
{
	auto newCar = Car::createFromFile("car.png");
	if (newCar)
	{
		newCar->m_cl = setup.GetClearance();
		newCar->SetEngine(engine);
		if (newCar->m_wheel_left && newCar->m_wheel_right)
		{
			newCar->m_wheel_left->SetWheel(setup.GetWheel());
			newCar->m_wheel_right->SetWheel(setup.GetWheel());
		}
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

float Car::GetSpeed() const
{
	auto speed = GetBody()->GetLinearVelocity();
	return sqrtf(speed.x * speed.x + speed.y * speed.y);
}

void Car::onEnter()
{
	if (m_wheel_left && m_wheel_right)
	{
		m_wheel_left->setPosition(m_wheel_left->getContentSize().width / 2.0 + LEFT_WHEEL_OFFSET_X,
			HIGH_CLEARANCE_OFFSET_LEFT_WHEEL);

		m_wheel_right->setPosition(getContentSize().width - (m_wheel_right->getContentSize().width / 2.0 + RIGHT_WHEEL_OFFSET_X),
			HIGH_CLEARANCE_OFFSET_RIGHT_WHEEL);

		if (m_cl == ClearanceSetup::LOW_CLEARANCE)
		{
			m_wheel_left->setPositionY(m_wheel_left->getPositionY() + LOW_CLEARANCE_OFFSET);
			m_wheel_right->setPositionY(m_wheel_right->getPositionY() + LOW_CLEARANCE_OFFSET);
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

		m_wheel_left->GetBody()->SetLinearDamping(MIN_LINEAR_DAMPING);
		break;
	case LEFT:
		m_springLeft->EnableMotor(true);
		m_springLeft->SetMotorSpeed(0);

		m_wheel_left->GetBody()->SetLinearDamping(MAX_LINEAR_DAMPING);
		break;
	case RIGHT:
		m_springLeft->EnableMotor(true);
		m_springLeft->SetMotorSpeed(-MAX_MOTOR_SPEED);

		m_wheel_left->GetBody()->SetLinearDamping(MIN_LINEAR_DAMPING);
		break;
	default:
		break;
	}
}

void Car::AddPoints(unsigned int count)
{
	m_bonusPoints += count;
}

unsigned int Car::GetPoints() const
{
	return m_bonusPoints;
}

void Car::SetContactWithTerrain(bool hasContact)
{
	m_terrainContact = hasContact;
}

bool Car::IsContactWithTerrain() const
{
	return m_terrainContact;
}

b2WheelJoint* Car::CreateWheelJoint(b2Body* body, Wheel* wheel, bool isEngineTurnOn)
{
	b2WheelJointDef	wheelDef;
	wheelDef.Initialize(body, wheel->GetBody(), wheel->GetBody()->GetPosition(), b2Vec2(0, 1));
	wheelDef.enableMotor = isEngineTurnOn;
	wheelDef.motorSpeed = 0;
	wheelDef.maxMotorTorque = MAX_MOTOR_TORQUE;
	wheelDef.dampingRatio = WHEEL_DAMPING_RATIO;
	wheelDef.frequencyHz = m_cl == ClearanceSetup::HIGH_CLEARANCE ? HIGH_CLEARANCE_FREQ : LOW_CLEARANCE_FREQ;

	return (b2WheelJoint*)body->GetWorld()->CreateJoint(&wheelDef);
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
	int count = 8;
	const b2Vec2 vertices[] = 
	{
		b2Vec2(-107.5 / ptmRatio, -31.0 / ptmRatio),
		b2Vec2(-107.5 / ptmRatio, 17.0 / ptmRatio),
		b2Vec2(-87.5 / ptmRatio, 13.0 / ptmRatio),
		b2Vec2(-37.5 / ptmRatio, 31.0 / ptmRatio),
		b2Vec2(2.5 / ptmRatio, 31.0 / ptmRatio),
		b2Vec2(25.5 / ptmRatio, 13.0 / ptmRatio),
		b2Vec2(107.5 / ptmRatio, 4.0 / ptmRatio),
		b2Vec2(107.5 / ptmRatio, -31.0 / ptmRatio)
	};
	shape.Set(vertices, count);

	b2FixtureDef fdef;
	fdef.shape = &shape;
	fdef.density = CAR_BODY_DENSITY;
	fdef.friction = CAR_BODY_FRICTION;
	fdef.restitution = 0;
	body->CreateFixture(&fdef);	

	m_springLeft = CreateWheelJoint(body, m_wheel_left, true);
	m_springRight = CreateWheelJoint(body, m_wheel_right, false);

	return Pointer<CarPuppeteer>(res);
}