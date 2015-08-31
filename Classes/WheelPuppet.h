#pragma once
#include "SceneObjectPuppet.h"
#include "SceneObjectPuppeteers.h"
#include "PhysicsEngine.h"
#include "CarSetup.h"

class Wheel : public SceneObjectPuppet < Wheel, WheelPuppeteer >
{
public:
	static Wheel* create();
	void SetWheel(WheelSetup type);

protected:
	Pointer<WheelPuppeteer> CreatePuppeteer(PhysicsEngine*) override;

private:
	WheelSetup m_wheelType;

	static const float BIG_WHEEL_SCALE;
	static const float SMALL_WHEEL_SCALE;

	static const float DENSITY;
	static const float FRICTION;
	static const float RESTITUTION;
	static const float LINEAR_DAMPING;
};

