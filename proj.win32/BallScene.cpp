#include "BallScene.h"
#include "BallPuppet.h"
//#include "GroundPuppet.h"
#include "TerrainSegment.h"
#include "TerrainSegmentData.h"
#include <ctime>

USING_NS_CC;

BallScene::BallScene()
	:m_engine(this, PIXELS_PER_METER)
{
}

Scene* BallScene::createScene()
{
	auto scene = Scene::create();
	auto layer = BallScene::create();
	
	scene->addChild(layer);
	
	return scene;
}

void BallScene::onEnter()
{
	Layer::onEnter();
	scheduleUpdate();
}

void BallScene::onExit()
{
	unscheduleUpdate();
	Layer::onExit();
}

void BallScene::update(float dt)
{
	m_engine.tick(dt);
}

bool BallScene::init()
{
	
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	TerrainSegmentData data;
	for (int i = -2; i < data.SEGMENT_LENGHT + 2; ++i)
	{
		std::srand(time(0));
		data.setKeypointAtIndex(i, (rand() % 11) / 10.0);
	}

	auto ball = Ball::create();
	//auto ground = Ground::create();

	ball->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - ball->getContentSize().height));
	//ground->setPosition(Vec2(origin.x + ground->getContentSize().width + 200, origin.y + ground->getContentSize().height));

	//ground->SetEngine(&m_engine);
	ball->SetEngine(&m_engine);

	auto terrain = TerrainSegment::create(data, Size(visibleSize.width / 2.0, visibleSize.height / 2.0), &m_engine);
	
	//this->addChild(ground, 1);
	
	this->addChild(terrain, 1);
	this->addChild(ball, 1);

	return true;
}