#include "GameOverScene.h"
#include "CarSetupScene.h"
#include "MainScene.h"
#include "ui/CocosGUI.h"
#include <string>
#include <iomanip>

USING_NS_CC;

const Color4B GameOverScene::BACKGROUND_GRADIENT_FROM_SUCCESS = Color4B(0, 102, 0, 255);
const Color4B GameOverScene::BACKGROUND_GRADIENT_FROM_FAIL = Color4B(204, 51, 51, 255);
const Color4B GameOverScene::BACKGROUND_GRADIENT_TO = Color4B(0, 0, 0, 255);
const Color4B GameOverScene::GAMEOVER_SHADOW_COLOR = Color4B(0, 102, 102, 200);
const Color4B GameOverScene::SCORE_SHADOW_COLOR = Color4B(51, 0, 51, 255);
const Color4F GameOverScene::START_PARTICLES_COLOR = Color4F(1, 1, 1, 1);
const Color4F GameOverScene::END_PARTICLES_COLOR = Color4F(1, 1, 1, 0);

const std::string GameOverScene::LABEL_SUCCESS = "Congratulations !";
const std::string GameOverScene::LABEL_TIME_OVER = "Time over !";
const std::string GameOverScene::LABEL_FAIL = "Crash !";

GameOverScene::GameOverScene(CarSetup &setup, unsigned int points, EndType type, int seconds, int maxSeconds)
	:m_setup(setup), m_points(points), m_type(type), m_seconds(seconds), m_maxSeconds(maxSeconds)
{
}

GameOverScene* GameOverScene::create(CarSetup &setup, unsigned int points, EndType type, int seconds, int maxSeconds)
{
	GameOverScene *pRet = new(std::nothrow) GameOverScene(setup, points, type, seconds, maxSeconds);
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

Scene* GameOverScene::createScene(CarSetup &setup, unsigned int points, EndType type, int seconds, int maxSeconds)
{
	auto scene = Scene::create();
	auto layer = GameOverScene::create(setup, points, type, seconds, maxSeconds);

	scene->addChild(layer);

	return scene;
}

void GameOverScene::onEnter()
{
	LayerGradient::onEnter();
	scheduleUpdate();
}

void GameOverScene::onExit()
{
	unscheduleUpdate();
	LayerGradient::onExit();
}

void GameOverScene::AddLabels()
{
	std::string mainCaption = "";
	switch (m_type)
	{
	case FAIL:
		mainCaption = LABEL_FAIL;
		break;
	case TIME_OVER:
		mainCaption = LABEL_TIME_OVER;
		break;
	case SUCCESS:
		mainCaption = LABEL_SUCCESS;
		break;
	}
	auto gameOverLabel = Label::createWithSystemFont(mainCaption, "Arial", MAIN_LABEL_FONT_SIZE);
	gameOverLabel->setPosition(m_origin.x + m_visibleSize.width / 2.0, m_origin.y + m_visibleSize.height / 2.0 + MAIN_LABEL_OFFSET_Y);
	gameOverLabel->enableShadow(GAMEOVER_SHADOW_COLOR);

	std::string results = "Score : " + std::to_string(m_points) + " x ";
	float multiplier = 1.0f;
	if (m_type == EndType::SUCCESS)
	{
		multiplier = 1 + (m_maxSeconds - m_seconds) / 10.0;
	}
	std::stringstream ss;
	ss << std::fixed << std::setprecision(1) << multiplier;
	std::string multStr(ss.str());
	results += multStr + " = ";
	m_points *= multiplier;
	results += std::to_string(m_points);

	if (m_type == EndType::SUCCESS)
	{
		int seconds = m_seconds % 60;
		std::string minuteString = "0" + std::to_string(m_seconds / 60),
			secondString = seconds > 9 ? std::to_string(seconds) : "0" + std::to_string(seconds);
		results += "\nTime - " + minuteString + ":" + secondString;
	}
	auto labelResults = Label::createWithSystemFont(results, "Arial", SCORE_LABEL_FONT_SIZE);
	labelResults->setPosition(m_origin.x + m_visibleSize.width / 2.0, m_origin.y + m_visibleSize.height / 2.0 + SCORE_LABEL_OFFSET_Y);
	labelResults->enableShadow(SCORE_SHADOW_COLOR);

	this->addChild(gameOverLabel);
	this->addChild(labelResults);
}

void GameOverScene::AddMenu()
{
	auto menu = Menu::create();
	menu->setPosition(m_origin.x + m_visibleSize.width / 2.0, m_origin.y + m_visibleSize.height / 2.0);

	auto actionToSetup = MenuItemFont::create("To car setup", [](Ref*)
	{
		auto carSetupScene = CarSetupScene::createScene();
		Director::getInstance()->replaceScene(carSetupScene);
	});

	auto actionToStart = MenuItemFont::create("To start", [&](Ref*)
	{
		auto mainScene = MainScene::createScene(m_setup);
		Director::getInstance()->replaceScene(mainScene);
	});
	actionToStart->setPositionY(ACTION_START_OFFSET_Y);

	menu->addChild(actionToSetup);
	menu->addChild(actionToStart);

	this->addChild(menu);
}

void GameOverScene::AddParticles()
{
	ParticleSystemQuad* particles[2];
	for (size_t i = 0; i < 2; ++i)
	{
		particles[i] = ParticleSystemQuad::create("particle_texture.plist");
		particles[i]->setTexture(TextureCache::getInstance()->addImage("coin.png"));
		particles[i]->setStartColor(START_PARTICLES_COLOR);
		particles[i]->setEndColor(END_PARTICLES_COLOR);
		this->addChild(particles[i]);
	}
	particles[0]->setPosition(m_visibleSize.width / 5.0, m_visibleSize.height / 2.0);
	particles[1]->setPosition(4 * m_visibleSize.width / 5.0, m_visibleSize.height / 2.0);
}

bool GameOverScene::init()
{
	auto backgroundStartColor = m_type == EndType::SUCCESS ? BACKGROUND_GRADIENT_FROM_SUCCESS : BACKGROUND_GRADIENT_FROM_FAIL;
	if (!LayerGradient::initWithColor(backgroundStartColor, BACKGROUND_GRADIENT_TO))
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	AddLabels();
	AddMenu();
	if (m_type == EndType::SUCCESS)
	{
		AddParticles();
	}

	return true;
}