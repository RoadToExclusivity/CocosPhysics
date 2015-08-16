#include "MainScene.h"
#include "BallPuppet.h"
#include "CarPuppet.h"
#include "WheelPuppet.h"
#include "TerrainSegment.h"
#include "TerrainSegmentData.h"
#include "SurfaceGenerator.h"
#include <ctime>

USING_NS_CC;

MainScene::MainScene(CarSetup &setup)
	:m_engine(this, PIXELS_PER_METER), m_setup(setup)
{
}

MainScene* MainScene::create(CarSetup &setup)
{
	MainScene *pRet = new(std::nothrow) MainScene(setup);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = NULL;
		return NULL;
	}
}

Scene* MainScene::createScene(CarSetup &setup)
{
	auto scene = Scene::create();
	auto layer = MainScene::create(setup);

	scene->addChild(layer);
	
	return scene;
}

void MainScene::onEnter()
{
	Layer::onEnter();
	scheduleUpdate();
}

void MainScene::onExit()
{
	unscheduleUpdate();
	Layer::onExit();
}

void MainScene::update(float dt)
{
	m_engine.tick(dt);

	/*Size visibleSize = Director::getInstance()->getVisibleSize();
	visibleSize.width *= 0.25;
	visibleSize.height *= 0.6;
	this->setPosition(ccpSub(visibleSize , m_car->getPosition()));*/
}

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto car = Car::create(m_setup);
	car->setPosition(Vec2(origin.x + visibleSize.width / 2 - 350, origin.y + visibleSize.height));
	car->SetEngine(&m_engine);

	SurfaceGenerator gen;
	auto data = gen.NextSegment(), data2 = gen.NextSegment();
	/*TerrainSegmentData data, data2;
	for (int i = -2; i <= TerrainSegmentData::SEGMENT_LENGTH + 2; ++i)
	{
		data.setKeypointAtIndex(i, 1);
		data2.setKeypointAtIndex(i, 1);
	}*/
	auto terrain1 = TerrainSegment::create(data.first, Size(visibleSize.width / 2.0, visibleSize.height / 2.0), &m_engine),
		terrain2 = TerrainSegment::create(data2.first, Size(visibleSize.width / 2.0, visibleSize.height / 2.0), &m_engine);
	auto box1 = terrain1->boundingBox(), box2 = terrain2->boundingBox();
	terrain1->setPosition(Vec2(origin.x - box1.origin.x, origin.y - box1.origin.y));

	//float segmentSize = data.second * (box1.size.height - visibleSize.height / 2.0);
	//Point pp0(0, data.first.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH - 1));
	//Point pp1(0, data.first.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH));
	//Point pp2(0, data.first.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH + 1));
	//Point pp3(0, data.first.getKeypointAtIndex(TerrainSegmentData::SEGMENT_LENGTH + 2));
	//Point point = ccCardinalSplineAt(pp0, pp1, pp2, pp3, 0, 0);
	//point.y *= visibleSize.height / 2.0;
	//point.y = visibleSize.height + box1.origin.y;

	terrain2->setPosition(Vec2(origin.x - box2.origin.x + box1.size.width, 
		origin.y - box2.origin.y - (box2.size.height - visibleSize.height / 2.0)));
	
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	this->addChild(terrain1);
	this->addChild(terrain2);
	this->addChild(car);

	//this->setPosition(200, 200);
	m_car = car;
	//this->runAction(Follow::create(car));

	return true;
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		m_car->ApplyForce(ForceDirection::RIGHT);
	}
	else
	{
		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
		{
			m_car->ApplyForce(ForceDirection::LEFT);
		}
	}
}

void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW || keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		m_car->ApplyForce(ForceDirection::STOP);
	}
}