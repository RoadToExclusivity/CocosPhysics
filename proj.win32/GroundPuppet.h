#pragma once

#include "SceneObjectPuppet.h"
#include "SceneObjectPuppeteers.h"
#include "PhysicsEngine.h"

class Ground : public SceneObjectPuppet < Ground, GroundPuppeteer >
{
public:
	static Ground* create();

protected:
	Pointer<GroundPuppeteer> CreatePuppeteer(PhysicsEngine*) override;
};