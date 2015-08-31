#pragma once
#include "cocos2d.h"

class Accelerator : public cocos2d::Sprite
{
public:
	static Accelerator* create(float);
	Accelerator(float);

	void update(float);
private:
	float m_maxAcceleration;
	cocos2d::DrawNode* m_mainRect, *m_accelRect;

	const float A_WIDTH = 50, A_HEIGHT = 100;
	const cocos2d::Color4B NORMAL_ACCELERATION_COLOR = cocos2d::Color4B(51, 153, 0, 255);
	const cocos2d::Color4B BORDER_COLOR = cocos2d::Color4B(0, 51, 102, 255);
};