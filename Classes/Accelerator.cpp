#include "Accelerator.h"

USING_NS_CC;

Accelerator::Accelerator(float maxAcceleration)
	:m_maxAcceleration(maxAcceleration), m_mainRect(DrawNode::create()), m_accelRect(DrawNode::create())
{
}

Accelerator* Accelerator::create(float maxAcceleration)
{
	Accelerator* sprite = new (std::nothrow) Accelerator(maxAcceleration);
	if (sprite && sprite->init())
	{
		sprite->autorelease();
		Vec2 vertices[] =
		{
			Vec2(0, 0),
			Vec2(0, sprite->A_HEIGHT),
			Vec2(sprite->A_WIDTH, sprite->A_HEIGHT),
			Vec2(sprite->A_WIDTH, 0)
		};
		Vec2 borders[] =
		{
			Vec2(0, sprite->A_HEIGHT / 2.0 - 1),
			Vec2(0, sprite->A_HEIGHT / 2.0 + 1),
			Vec2(sprite->A_WIDTH, sprite->A_HEIGHT / 2.0 + 1),
			Vec2(sprite->A_WIDTH, sprite->A_HEIGHT / 2.0 - 1)
		};

		sprite->m_mainRect->drawPolygon(vertices, 4, Color4F(1, 1, 1, 1), 2, Color4F(sprite->BORDER_COLOR));
		sprite->m_mainRect->drawPolygon(borders, 4, Color4F(1, 1, 1, 1), 1, Color4F(sprite->BORDER_COLOR));

		sprite->addChild(sprite->m_mainRect);
		sprite->addChild(sprite->m_accelRect);

		return sprite;
	}

	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Accelerator::update(float acceleration)
{
	if (acceleration == 0)
	{
		m_accelRect->clear();
		return;
	}

	bool isBoost = true;
	if (fabsf(acceleration) > m_maxAcceleration)
	{
		isBoost = (acceleration > 0);
		acceleration = m_maxAcceleration;
	}

	float height = A_HEIGHT * (0.5 * acceleration / m_maxAcceleration);
	Vec2 vertices[4];
	if (isBoost)
	{
		vertices[0] = Vec2(1, A_HEIGHT / 2.0 + 0.5);
		vertices[1] = Vec2(1, A_HEIGHT / 2.0 + height - 1);
		vertices[2] = Vec2(A_WIDTH - 1, A_HEIGHT / 2.0 + height - 1);
		vertices[3] = Vec2(A_WIDTH - 1, A_HEIGHT / 2.0);
	}
	else
	{
		vertices[0] = Vec2(1, A_HEIGHT / 2.0 + 0.5);
		vertices[1] = Vec2(1, A_HEIGHT / 2.0 - height + 1);
		vertices[2] = Vec2(A_WIDTH - 1, A_HEIGHT / 2.0 - height + 1);
		vertices[3] = Vec2(A_WIDTH - 1, A_HEIGHT / 2.0 + 0.5);
	}

	auto curColor = NORMAL_ACCELERATION_COLOR;

	if (2 * acceleration > m_maxAcceleration)
	{
		int red = curColor.r;
		int green = curColor.g;
		float redRatio = (255 - red) * 2.0 / m_maxAcceleration;
		float greenRatio = green * 2.0 / m_maxAcceleration;

		red += (acceleration - m_maxAcceleration / 2.0) * redRatio;
		green -= (acceleration - m_maxAcceleration / 2.0) * greenRatio;

		curColor.r = red;
		curColor.g = green;
	}

	m_accelRect->clear();
	m_accelRect->drawPolygon(vertices, 4, Color4F(curColor), 0, Color4F(0, 0, 0, 0));
}
