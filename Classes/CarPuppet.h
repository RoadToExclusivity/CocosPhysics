#pragma once
#include "SceneObjectPuppet.h"
#include "SceneObjectPuppeteers.h"
#include "PhysicsEngine.h"
#include "WheelPuppet.h"
#include "CarPuppeteer.h"
#include "CarSetup.h"

enum ForceDirection
{
	STOP,
	LEFT,
	RIGHT
};

class Car : public SceneObjectPuppet < Car, CarPuppeteer >
{
public:
	Car();
	static Car* create(CarSetup &setup, PhysicsEngine* engine);

	void SetEngine(PhysicsEngine* engine) override;
	void onEnter() override;

	void ApplyForce(ForceDirection);
	void AddPoints(unsigned int count);

	float GetSpeed() const;
	unsigned int GetPoints() const;

	void SetContactWithTerrain(bool);
	bool IsContactWithTerrain() const;

protected:
	Pointer<CarPuppeteer> CreatePuppeteer(PhysicsEngine*) override;

private:
	b2WheelJoint* CreateWheelJoint(b2Body*, Wheel*, bool);

	Wheel *m_wheel_left = nullptr, *m_wheel_right = nullptr;
	b2WheelJoint* m_springLeft = nullptr, *m_springRight = nullptr;
	ClearanceSetup m_cl = ClearanceSetup::LOW_CLEARANCE;
	
	unsigned int m_bonusPoints = 0;
	bool m_terrainContact = false;

	const float MAX_MOTOR_TORQUE = 50.0f;
	const float MAX_MOTOR_SPEED = 250.0f;
	const float MAX_LINEAR_DAMPING = 25.0f;
	const float MIN_LINEAR_DAMPING = 0.1f;
	const float WHEEL_DAMPING_RATIO = 0.7f;
	const float LOW_CLEARANCE_OFFSET = 8.0f;
	const float HIGH_CLEARANCE_OFFSET_LEFT_WHEEL = -2;
	const float HIGH_CLEARANCE_OFFSET_RIGHT_WHEEL = 0;
	const float CAR_BODY_FRICTION = 0.9f;
	const float CAR_BODY_DENSITY = 1.0f;
	const float LEFT_WHEEL_OFFSET_X = 12.0f;
	const float RIGHT_WHEEL_OFFSET_X = 14.0f;
	const float HIGH_CLEARANCE_FREQ = 4.0f;
	const float LOW_CLEARANCE_FREQ = 25.0f;
};

