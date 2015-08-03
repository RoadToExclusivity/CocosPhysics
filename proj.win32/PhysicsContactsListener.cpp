#include "PhysicsContactsListener.h"

void PhysicsContactsListener::BeginContact(b2Contact *contact)
{
    b2Body *bodyA = contact->GetFixtureA()->GetBody();
    b2Body *bodyB = contact->GetFixtureB()->GetBody();
    PhysicsPuppeteer *puppeteerA = reinterpret_cast<PhysicsPuppeteer *>(bodyA->GetUserData());
    PhysicsPuppeteer *puppeteerB = reinterpret_cast<PhysicsPuppeteer *>(bodyB->GetUserData());

    if (puppeteerA == nullptr && puppeteerB == nullptr)
        return;

    PhysicsContactInfo info;
    info.myFixture = contact->GetFixtureA();
    info.theirFixture = contact->GetFixtureB();
    info.theirPuppeteer = puppeteerB;

    if (puppeteerA)
        puppeteerA->didBeginContact(info);

    std::swap(info.myFixture, info.theirFixture);
    info.theirPuppeteer = puppeteerA;

    if (puppeteerB)
        puppeteerB->didBeginContact(info);
}

void PhysicsContactsListener::EndContact(b2Contact *contact)
{
    b2Body *bodyA = contact->GetFixtureA()->GetBody();
    b2Body *bodyB = contact->GetFixtureB()->GetBody();
    PhysicsPuppeteer *puppeteerA = reinterpret_cast<PhysicsPuppeteer *>(bodyA->GetUserData());
    PhysicsPuppeteer *puppeteerB = reinterpret_cast<PhysicsPuppeteer *>(bodyB->GetUserData());

    if (puppeteerA == nullptr && puppeteerB == nullptr)
        return;

    PhysicsContactInfo info;
    info.myFixture = contact->GetFixtureA();
    info.theirFixture = contact->GetFixtureB();
    info.theirPuppeteer = puppeteerB;

    if (puppeteerA)
        puppeteerA->didEndContact(info);

    std::swap(info.myFixture, info.theirFixture);
    info.theirPuppeteer = puppeteerA;

    if (puppeteerB)
        puppeteerB->didEndContact(info);
}
