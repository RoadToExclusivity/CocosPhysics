#include "NodePhysicsPuppeteer.h"
#include "PhysicsEngine.h"

NodePhysicsPuppeteer *NodePhysicsPuppeteer::create(cocos2d::Node *node, const b2BodyDef &bodyDef, PhysicsEngine *engine)
{
    NodePhysicsPuppeteer *ret = new NodePhysicsPuppeteer();
    ret->init(node, bodyDef, engine);
    ret->autorelease();
    return ret;
}

void NodePhysicsPuppeteer::didUpdatePhysics()
{
    m_node->setPosition(getEngine()->getBodyPosition(getBody()));
    m_node->setRotation(getEngine()->getBodyRotation(getBody()));
}

void NodePhysicsPuppeteer::willUpdatePhysics(float dt)
{
}

NodePhysicsPuppeteer::NodePhysicsPuppeteer()
: m_node(nullptr)
{
}

void NodePhysicsPuppeteer::init(cocos2d::Node *node, const b2BodyDef &bodyDef, PhysicsEngine *engine)
{
    PhysicsPuppeteer::init(bodyDef, engine);
    m_node = node;
}

cocos2d::CCNode *NodePhysicsPuppeteer::getNode() const
{
    return m_node;
}
