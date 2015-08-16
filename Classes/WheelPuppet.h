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
};

