#include "MainScene.h"
#include "CarPuppet.h"
#include "WheelPuppet.h"
#include "TerrainSegment.h"
#include "TerrainSegmentData.h"
#include "SurfaceGenerator.h"
#include "ui/CocosGUI.h"
#include "GameOverScene.h"
#include "Accelerator.h"
#include "Speedometer.h"
#include "BonusPuppet.h"
#include "BonusPositionGenerator.h"

USING_NS_CC;

const float MainScene::MAX_ACCELERATION = 6.0f;
const float MainScene::CAMERA_X_RATIO = 0.4f;
const float MainScene::CAMERA_Y_RATIO = 0.5f;
const float MainScene::MIN_ANGULAR_CAPSIZE_SPEED = 0.5f;
const float MainScene::MIN_LINEAR_CAPSIZE_SPEED = 0.5f;

const Color4B MainScene::SKY_COLOR = Color4B(51, 153, 255, 255);
const Color3B MainScene::LABEL_MAX_TIME = Color3B(102, 102, 102);

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

	delete[] terrains;
}

bool MainScene::AddBonuses()
{
	int bonusCount = (rand() % (MAX_BONUS_COUNT - MIN_BONUS_COUNT + 1)) + MIN_BONUS_COUNT;
	float maxBonusDistance = 0;
	float distance = BONUSES_START_OFFSET, maxDistance = TERRAIN_COUNT * m_terrainWidth;
	BonusPositionGenerator gen;
	for (int i = 0; i < bonusCount; ++i)
	{
		const int BONUSES_POSSIBILITY_COUNT = 3 + 2 + 1;
		int bonusType = rand() % BONUSES_POSSIBILITY_COUNT;
		bonusType = bonusType < 3 ? 0 : 
					bonusType < 5 ? 1 : 2;

		auto bonus = Bonus::create(BonusType(bonusType), &m_engine);
		if (!bonus)
		{
			return false;
		}

		float offset = 0;
		if (i > 0)
		{
			if (i < bonusCount - 1)
			{
				maxBonusDistance = 1.0f * (maxDistance - distance - (bonusCount - i) * BONUSES_SIZE) / (bonusCount - i - 1);
				offset = BONUSES_SIZE + gen.NextPosition(maxBonusDistance);
			}
			else
			{
				maxBonusDistance = 1.0f * (maxDistance - distance - BONUSES_SIZE / 2.0f);
				offset = BONUSES_SIZE / 2.0f + gen.NextPosition(maxBonusDistance);
			}

			CC_ASSERT(maxBonusDistance > 0);
			CC_ASSERT(offset > 0);
		}
		distance += offset;
		CC_ASSERT(distance < maxDistance);

		size_t segmentIndex = distance / m_terrainWidth;
		float bonusXPosition = distance - m_terrainWidth * segmentIndex;
		float bonusYPosition = terrains[segmentIndex]->getSurfaceY(bonusXPosition) + terrains[segmentIndex]->getPositionY()
								- m_terrainHeight / 2.0 + BONUS_OFFSET_Y;

		bonus->setPosition(distance, bonusYPosition);

		this->addChild(bonus, 2);
	}

	return true;
}

bool MainScene::AddBackground()
{
	m_background = LayerColor::create(SKY_COLOR);
	if (!m_background)
	{
		return false;
	}

	m_background->setPosition(m_origin.x, m_origin.y);
	this->addChild(m_background, -1	);
	
	return true;
}

bool MainScene::AddCar()
{
	m_car = Car::create(m_setup, &m_engine);
	if (!m_car)
	{
		return false;
	}

	m_car->setPosition(Vec2(m_origin.x + m_visibleSize.width / 2 - CAR_START_OFFSET_X, m_origin.y + terrains[0]->getSurfaceY(0)
						+ terrains[0]->getPositionY() - m_terrainHeight / 2.0 + CAR_START_OFFSET_Y));

	this->addChild(m_car, 1);

	return true;
}

bool MainScene::AddFinishBorder()
{
	auto finish = Sprite::create("finish.png");
	if (!finish)
	{
		return false;
	}

	finish->setAnchorPoint(Vec2(0, 0));
	finish->setPosition(TERRAIN_COUNT * m_terrainWidth, terrains[TERRAIN_COUNT]->getSurfaceY(0) +
						terrains[TERRAIN_COUNT]->getPositionY() - m_terrainHeight / 2.0);

	this->addChild(finish);

	return true;
}

bool MainScene::AddMeters()
{
	m_sp = Speedometer::create(m_visibleSize.width / 2.0);
	m_accel = Accelerator::create(MAX_ACCELERATION * m_engine.getPtmRatio());
	m_points = Label::createWithSystemFont("Points : 0", "Arial", LABELS_FONT_SIZE);
	m_time = Label::createWithSystemFont("00:00", "Arial", LABELS_FONT_SIZE);

	std::string minutes = std::to_string(MAX_TIME_MINUTES),
		seconds = MAX_TIME_SECONDS > 9 ? std::to_string(MAX_TIME_SECONDS) : "0" + std::to_string(MAX_TIME_SECONDS);
	m_maxTimeLabel = Label::createWithSystemFont("0" + minutes + ":" + seconds, "Arial", LABEL_MAX_TIME_FONT_SIZE);
	if (!m_sp || !m_accel || !m_points || !m_time || !m_maxTimeLabel)
	{
		return false;
	}

	m_sp->setPosition(SPEEDOMETER_X, METERS_Y);
	m_accel->setPosition(ACCELERATOR_X, METERS_Y);

	m_points->setAnchorPoint(Vec2(0, 1));
	m_points->setPosition(0, m_visibleSize.height);

	m_time->setAnchorPoint(Vec2(1, 1));
	m_time->setPosition(m_visibleSize.width, m_visibleSize.height);
	
	m_maxTimeLabel->setAnchorPoint(Vec2(1, 1));
	m_maxTimeLabel->setPosition(m_visibleSize.width, m_visibleSize.height);
	m_maxTimeLabel->setColor(LABEL_MAX_TIME);

	m_startTime = clock();

	this->addChild(m_sp, 2);
	this->addChild(m_accel, 2);
	this->addChild(m_points, 2);
	this->addChild(m_time, 2);
	this->addChild(m_maxTimeLabel, 2);

	return true;
}

bool MainScene::AddTerrain()
{
	terrains = new TerrainSegment*[TERRAIN_COUNT + END_FLAT_TERRAINS];
	if (!terrains)
	{
		return false;
	}

	SurfaceGenerator gen;
	m_terrainWidth = m_visibleSize.width / 2.0;
	m_terrainHeight = m_visibleSize.height / 2.0;
	float prevRectWidth = 0;

	for (size_t i = 0; i < TERRAIN_COUNT + END_FLAT_TERRAINS; ++i)
	{
		auto terrainData = i < TERRAIN_COUNT ? gen.NextSegment() : gen.FlatSegment();
		auto info = terrainData.second;

		terrains[i] = TerrainSegment::create(terrainData.first, info,
						Size(m_terrainWidth, m_terrainHeight), &m_engine);
		if (!terrains[i])
		{
			return false;
		}

		auto terrain = terrains[i];

		auto curBox = terrain->boundingBox();

		if (i > 0)
		{
			prevRectWidth += curBox.size.width;
			float curY = terrains[i - 1]->getSurfaceY(m_terrainWidth) + terrains[i - 1]->getPositionY() - terrain->getSurfaceY(0);
			terrain->setPosition(m_origin.x - curBox.origin.x + prevRectWidth, m_origin.y + curY);
		}
		else
		{
			terrain->setPosition(m_origin.x - curBox.origin.x,
				m_origin.y - curBox.origin.y);
		}

		this->addChild(terrain);
	}

	return true;
}

bool MainScene::BindControl()
{
	auto keyboardListener = EventListenerKeyboard::create();
	if (!keyboardListener)
	{
		return false;
	}

	keyboardListener->onKeyPressed = CC_CALLBACK_2(MainScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

	return true;
}

void MainScene::SetCameraAtCar()
{
	Vec2 camPos = Vec2(m_visibleSize);
	camPos.x *= CAMERA_X_RATIO;
	camPos.y *= CAMERA_Y_RATIO;
	camPos -= m_car->getPosition();
	if (camPos.x > 0)
	{
		camPos.x = 0;
	}

	this->setPosition(camPos);

	m_sp->setPosition(-getPosition().x + SPEEDOMETER_X, -getPosition().y + METERS_Y);
	m_accel->setPosition(-getPosition().x + ACCELERATOR_X, -getPosition().y + METERS_Y);
	m_points->setPosition(-getPosition().x, -getPosition().y + m_visibleSize.height);
	m_time->setPosition(-getPosition().x + m_visibleSize.width, -getPosition().y + m_visibleSize.height);
	m_maxTimeLabel->setPosition(-getPosition().x + m_visibleSize.width, -getPosition().y + m_visibleSize.height - 
																		m_time->getContentSize().height - LABEL_MAX_TIME_OFFSET_Y);

	m_background->setPosition(-getPosition().x, -getPosition().y);
}

void MainScene::GetTime(int& minutes, int& seconds) const
{
	int summSeconds = (1.0f * clock() - m_startTime) / CLOCKS_PER_SEC;
	
	minutes = summSeconds / 60;
	seconds = summSeconds % 60;
}

void MainScene::UpdateMeters(float dt)
{
	float speed = m_car->GetSpeed() * m_engine.getPtmRatio();
	m_sp->update(speed);
	m_accel->update((speed - m_prevSpeed) / dt);
	m_prevSpeed = speed;

	m_points->setString("Points : " + std::to_string(m_car->GetPoints()));

	int minutes, seconds;
	GetTime(minutes, seconds);
	std::string minuteString = "0" + std::to_string(minutes), 
				secondString = seconds > 9 ? std::to_string(seconds) : "0" + std::to_string(seconds);
	m_time->setString(minuteString + ":" + secondString);
}

void MainScene::update(float dt)
{
	m_engine.tick(dt);

	if (ReachedEnd())
	{
		EndGame(EndType::SUCCESS);
		return;
	}

	if (IsCapsized())
	{
		EndGame(EndType::FAIL);
		return;
	}

	if (IsLate())
	{
		EndGame(EndType::TIME_OVER);
		return;
	}
	
	UpdateMeters(dt);
	SetCameraAtCar();
}

bool MainScene::ReachedEnd() const
{
	return m_car->getPositionX() - CAR_END_OFFSET_X > TERRAIN_COUNT * m_terrainWidth;
}

bool MainScene::IsLate() const
{
	int minutes, seconds;
	GetTime(minutes, seconds);

	return (minutes * 60 + seconds > MAX_TIME_MINUTES * 60 + MAX_TIME_SECONDS);
}

bool MainScene::IsCapsized() const
{
	auto angle = m_car->getRotation();
	auto speed = m_car->GetSpeed();
	auto angularSpeed = fabsf(m_car->GetBody()->GetAngularVelocity());

	return (m_car->IsContactWithTerrain() && angularSpeed < MIN_ANGULAR_CAPSIZE_SPEED &&
		speed < MIN_LINEAR_CAPSIZE_SPEED && (angle > MAX_ANGLE_CAPSIZE || angle < -MAX_ANGLE_CAPSIZE));
}

void MainScene::EndGame(EndType type)
{
	int minutes, seconds;
	GetTime(minutes, seconds);
	auto gameOverScene = GameOverScene::createScene(m_setup, m_car->GetPoints(), type, 
													minutes * 60 + seconds, MAX_TIME_MINUTES * 60 + MAX_TIME_SECONDS);
	Director::getInstance()->replaceScene(gameOverScene);
}

bool MainScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	if (!AddBackground() || !AddTerrain() || !AddCar() || !AddBonuses() || !AddMeters() || !AddFinishBorder() || !BindControl())
	{
		return false;
	}

	return true;
}

void MainScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (keyCode == EventKeyboard::KeyCode::KEY_Q)
	{
		EndGame(EndType::FAIL);
		return;
	}

	if (keyCode == EventKeyboard::KeyCode::KEY_R)
	{
		auto car = m_car->GetBody();
		auto pos = car->GetPosition();
		pos.y += 1.5;
		car->SetTransform(pos, 0);
		
		return;
	}

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