#include "Speedometer.h"

USING_NS_CC;

Speedometer::Speedometer(float maxSpeed)
	:m_maxSpeed(maxSpeed), m_mainRect(DrawNode::create()), m_speedRect(DrawNode::create())
{
}

Speedometer* Speedometer::create(float maxSpeed)
{
	Speedometer* sprite = new (std::nothrow) Speedometer(maxSpeed);
	if (sprite && sprite->init())
	{
		sprite->autorelease();
		Vec2 vertices[] =
		{
			Vec2(0, 0),
			Vec2(0, sprite->S_HEIGHT),
			Vec2(sprite->S_WIDTH, sprite->S_HEIGHT),
			Vec2(sprite->S_WIDTH, 0)
		};
		sprite->m_mainRect->drawPolygon(vertices, 4, Color4F(1, 1, 1, 1), 2, Color4F(sprite->BORDER_COLOR));

		sprite->addChild(sprite->m_mainRect);
		sprite->addChild(sprite->m_speedRect);

		return sprite;
	}

	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Speedometer::update(float speed)
{
	if (speed > m_maxSpeed)
	{
		speed = m_maxSpeed;
	}

	float height = S_HEIGHT * (1.0 * speed / m_maxSpeed);
	Vec2 vertices[] =
	{
		Vec2(1, 1),
		Vec2(1, height - 1),
		Vec2(S_WIDTH - 1, height - 1),
		Vec2(S_WIDTH - 1, 1)
	};

	auto curColor = NORMAL_SPEED_COLOR;
	
	if (2 * speed > m_maxSpeed)
	{
		int red = curColor.r;
		int green = curColor.g;
		float redRatio = (255 - red) * 2.0 / m_maxSpeed;
		float greenRatio = green * 2.0 / m_maxSpeed;
		
		red += (speed - m_maxSpeed / 2.0) * redRatio;
		green -= (speed - m_maxSpeed / 2.0) * greenRatio;

		curColor.r = red;
		curColor.g = green;
	}

	m_speedRect->clear();
	m_speedRect->drawPolygon(vertices, 4, Color4F(curColor), 0, Color4F(0, 0, 0, 0));
}