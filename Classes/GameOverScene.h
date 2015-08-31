#pragma once
#include "cocos2d.h"
#include "CarSetup.h"

enum EndType
{
	FAIL,
	TIME_OVER,
	SUCCESS
};

class GameOverScene : public cocos2d::LayerGradient
{
public:
	GameOverScene(CarSetup&, unsigned int, EndType, int, int);
	static cocos2d::Scene* createScene(CarSetup&, unsigned int, EndType, int, int);
	static GameOverScene* create(CarSetup&, unsigned int, EndType, int, int);

	bool init() override;
	void onEnter() override;
	void onExit() override;
	
	void AddMenu();
	void AddLabels();
	void AddParticles();

private:
	CarSetup m_setup;
	unsigned int m_points;
	EndType m_type;
	int m_seconds;
	int m_maxSeconds;

	cocos2d::Size m_visibleSize;
	cocos2d::Vec2 m_origin;

	static const int MAIN_LABEL_FONT_SIZE = 36;
	static const int SCORE_LABEL_FONT_SIZE = 32;
	static const int MAIN_LABEL_OFFSET_Y = 200;
	static const int SCORE_LABEL_OFFSET_Y = 100;
	static const int ACTION_START_OFFSET_Y = -50;

	static const cocos2d::Color4B BACKGROUND_GRADIENT_FROM_SUCCESS;
	static const cocos2d::Color4B BACKGROUND_GRADIENT_FROM_FAIL;
	static const cocos2d::Color4B BACKGROUND_GRADIENT_TO;
	static const cocos2d::Color4B GAMEOVER_SHADOW_COLOR;
	static const cocos2d::Color4B SCORE_SHADOW_COLOR;
	static const cocos2d::Color4F START_PARTICLES_COLOR;
	static const cocos2d::Color4F END_PARTICLES_COLOR;

	static const std::string LABEL_SUCCESS;
	static const std::string LABEL_TIME_OVER;
	static const std::string LABEL_FAIL;
};

