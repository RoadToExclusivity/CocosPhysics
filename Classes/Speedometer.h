#pragma once
#include "cocos2d.h"

class Speedometer : public cocos2d::Sprite
{
public:
	static Speedometer* create(float);
	Speedometer(float);

	void update(float);
private:
	float m_maxSpeed;
	cocos2d::DrawNode* m_mainRect, *m_speedRect;

	const float S_WIDTH = 50, S_HEIGHT = 100;
	const cocos2d::Color4B NORMAL_SPEED_COLOR = cocos2d::Color4B(51, 153, 0, 255);
	const cocos2d::Color4B BORDER_COLOR = cocos2d::Color4B(0, 51, 102, 255);
};

