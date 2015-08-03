#pragma once
#include "PhysicsPuppeteer.h"
#include "cocos2d.h"

class NodePhysicsPuppeteer : public PhysicsPuppeteer
{
public:
    static NodePhysicsPuppeteer *create(cocos2d::Node *node, const b2BodyDef &bodyDef, PhysicsEngine *engine);
    void didUpdatePhysics() override;
    void willUpdatePhysics(float dt) override;

protected:
    NodePhysicsPuppeteer();
    void init(cocos2d::Node *node, const b2BodyDef &bodyDef, PhysicsEngine *engine);
    cocos2d::Node *getNode() const;

private:
    cocos2d::Node *m_node;
};
