#pragma once

#include "SceneObjectPuppet.h"
#include "SceneObjectPuppeteers.h"
#include "PhysicsEngine.h"

class Ball : public SceneObjectPuppet < Ball, BallPuppeteer >
{
public:
	static Ball* create();

protected:
	Pointer<BallPuppeteer> CreatePuppeteer(PhysicsEngine*) override;
};