#pragma once
#include "SceneObjectPuppet.h"
#include "SceneObjectPuppeteers.h"
#include "PhysicsEngine.h"
#include "WheelPuppet.h"
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
	static Car* create(CarSetup &setup);

	void SetEngine(PhysicsEngine* engine) override;
	void onEnter() override;

	void ApplyForce(ForceDirection);

protected:
	Pointer<CarPuppeteer> CreatePuppeteer(PhysicsEngine*) override;

private:
	Wheel *m_wheel_left = nullptr, *m_wheel_right = nullptr;
	b2WheelJoint* m_springLeft = nullptr, *m_springRight = nullptr;
	ClearanceSetup m_cl = ClearanceSetup::LOW_CLEARANCE;
	
	const float MAX_MOTOR_TORQUE = 35.0f;
	const float MAX_MOTOR_SPEED = 150.0f;
	const float MAX_LINEAR_DAMPING = 25.0f;
};

