#pragma once

#include "cocos2d.h"
#include "Box2D\Box2D.h"

class PhysicsEngine
{
public:
    PhysicsEngine(cocos2d::Layer *parent, int pixelsPerMeter);
    ~PhysicsEngine();

    b2Body *createBody(const b2BodyDef &bodyDef) const;
    void deleteBodyLater(b2Body *body);

    /// Updates physics world.
    void tick(float dt);

    cocos2d::Point getGlobalBodyPosition(b2Body *body) const;
	cocos2d::Vec2 getLocalBodyPosition(b2Body *body, cocos2d::Node*) const;
	float getLocalBodyRotation(b2Body *body, cocos2d::Node*) const;
    float getGlobalBodyRotation(b2Body *body) const;
    float getCocosAngleFromBox2d(float angle) const;
    float getBox2dAngleFromCocos(float angle) const;
    int getPtmRatio() const;

    b2Vec2 getNodePosition(cocos2d::Node *node) const;
    b2Vec2 getNodePointPosition(cocos2d::Node *node, const cocos2d::Point &point) const;
	cocos2d::Mat4 getNodeToLocalTransform(cocos2d::Node*) const;

private:
	b2World *mWorld;
    std::set<b2Body *> mBodiesToDelete;
    int mPixelsPerMeter;
	cocos2d::Layer *m_parent = nullptr;
};
