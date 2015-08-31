#pragma once

#include "cocos2d.h"
#include "PhysicsEngine.h"
#include "CarPuppet.h"
#include "CarSetup.h"
#include "Speedometer.h"
#include "Accelerator.h"
#include "BonusPuppet.h"
#include "TerrainSegment.h"
#include "GameOverScene.h"

class MainScene : public cocos2d::Layer
{
public:
	MainScene(CarSetup&);
	static cocos2d::Scene* createScene(CarSetup&);
	static MainScene* create(CarSetup&);

	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float) override;

private:
	PhysicsEngine m_engine;
	CarSetup m_setup;

	Car* m_car = nullptr;
	TerrainSegment** terrains = nullptr;
	Speedometer* m_sp = nullptr;
	Accelerator* m_accel = nullptr;
	cocos2d::LayerColor* m_background = nullptr;
	cocos2d::Label* m_points = nullptr;
	cocos2d::Label* m_time = nullptr;
	cocos2d::Label* m_maxTimeLabel = nullptr;

	cocos2d::Size m_visibleSize;
	cocos2d::Vec2 m_origin;

	float m_prevSpeed = 0.0f, m_terrainWidth = 0.0f, m_terrainHeight = 0.0f;
	float m_startTime = 0.0f;

	static const float MAX_ACCELERATION;
	static const int PIXELS_PER_METER = 30;
	static const size_t TERRAIN_COUNT = 20;
	static const int SPEEDOMETER_X = 100;
	static const int ACCELERATOR_X = 25;
	static const int METERS_Y = 2;
	static const int MIN_BONUS_COUNT = TERRAIN_COUNT * 1;
	static const int MAX_BONUS_COUNT = TERRAIN_COUNT * 2;
	static const int BONUSES_START_OFFSET = 350;
	static const int END_FLAT_TERRAINS = 3;
	static const int MAX_ANGLE_CAPSIZE = 160;
	static const int MAX_TIME_MINUTES = 1;
	static const int MAX_TIME_SECONDS = 0;
	static const int CAR_START_OFFSET_X = 350;
	static const int CAR_START_OFFSET_Y = 100;
	static const int CAR_END_OFFSET_X = 140;
	static const int BONUS_OFFSET_Y = 40;
	static const int BONUSES_SIZE = 40;
	static const int LABELS_FONT_SIZE = 30;
	static const int LABEL_MAX_TIME_FONT_SIZE = 32;
	static const int LABEL_MAX_TIME_OFFSET_Y = 0;

	static const float CAMERA_X_RATIO;
	static const float CAMERA_Y_RATIO;
	static const float MIN_ANGULAR_CAPSIZE_SPEED;
	static const float MIN_LINEAR_CAPSIZE_SPEED;

	static const cocos2d::Color4B SKY_COLOR;
	static const cocos2d::Color3B LABEL_MAX_TIME;

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode, cocos2d::Event*);

	bool AddBackground();
	bool AddBonuses();
	bool AddCar();
	bool AddFinishBorder();
	bool AddMeters();
	bool AddTerrain();
	bool BindControl();
	void SetCameraAtCar();
	void UpdateMeters(float);

	bool ReachedEnd() const;
	bool IsCapsized() const;
	bool IsLate() const;
	void EndGame(EndType);

	void GetTime(int& minutes, int& seconds) const;
};