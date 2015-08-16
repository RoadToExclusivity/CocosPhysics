#include "PhysicsPuppeteer.h"
#include "PhysicsEngine.h"

PhysicsPuppeteer::PhysicsPuppeteer()
{
}

PhysicsPuppeteer::~PhysicsPuppeteer()
{
    if (m_body)
    {
        m_engine->deleteBodyLater(m_body);
        m_body = nullptr;
    }
}

void PhysicsPuppeteer::init(const b2BodyDef &bodyDef, PhysicsEngine *engine)
{
    m_engine = engine;
    b2BodyDef fixedDef = bodyDef;
    fixedDef.userData = reinterpret_cast<void *>(this);
    m_body = m_engine->createBody(fixedDef);
}

PhysicsEngine *PhysicsPuppeteer::getEngine() const
{
    return m_engine;
}

b2Body *PhysicsPuppeteer::getBody() const
{
    return m_body;
}
