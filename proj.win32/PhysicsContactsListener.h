#pragma once

#include "cocos2d.h"
#include "PhysicsPuppeteer.h"

class PhysicsContactsListener : public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
};
