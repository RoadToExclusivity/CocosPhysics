#pragma once

#include "cocos2d.h"
#include "Box2D\Box2D.h"

class PhysicsPuppeteer;
class PhysicsEngine;

class PhysicsContactInfo
{
public:
    b2Fixture *myFixture;             ///< Cannot be null.
    b2Fixture *theirFixture;          ///< Cannot be null.
    PhysicsPuppeteer *theirPuppeteer; ///< Is null if and only if object has no puppeteer.

    template <class T>
    bool isClass() const
    {
        return (nullptr != dynamic_cast<T *>(theirPuppeteer));
    }
};

class PhysicsPuppeteer : public cocos2d::Object
{
public:
    PhysicsPuppeteer();
    ~PhysicsPuppeteer();
	
    void init(const b2BodyDef &bodyDef, PhysicsEngine *engine);

    PhysicsEngine *getEngine() const;
    b2Body *getBody() const;

    virtual void didUpdatePhysics() = 0;
    virtual void willUpdatePhysics(float dt) = 0;

    virtual void didBeginContact(const PhysicsContactInfo &info) { CC_UNUSED_PARAM(info); }
    virtual void didEndContact(const PhysicsContactInfo &info) { CC_UNUSED_PARAM(info); }

private:
    b2Body *m_body = nullptr;
    PhysicsEngine *m_engine = nullptr;
};
