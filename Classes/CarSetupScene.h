#pragma once
#include "cocos2d.h"
#include "ui\CocosGUI.h"
#include "CarSetup.h"
#include "PhysicsEngine.h"
#include "CarPuppet.h"

class CarSetupScene : public cocos2d::LayerGradient
{
public:
	static cocos2d::Scene* createScene();
	bool init() override;
	void onEnter() override;
	void onExit() override;
	void update(float) override;

	void onContinue(cocos2d::Ref*, cocos2d::ui::Widget::TouchEventType);

	CREATE_FUNC(CarSetupScene);
private:
	CarSetupScene();
	cocos2d::ui::Button **buttons = nullptr;
	Car* m_car = nullptr;
	PhysicsEngine m_engine;
	CarSetup m_setup;
	int m_selectedButton = -1;
	cocos2d::Vec2 m_origin;
	cocos2d::Size m_visibleSize;

	static const int ENGINE_PTM = 60;

	template<WheelSetup w, ClearanceSetup c, int index>
	void CarSettingEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
	{
		switch (type)
		{
		case Widget::TouchEventType::ENDED:
		{
			m_setup.SetWheel(w);
			m_setup.SetClearance(c);

			this->removeChild(m_car);

			m_car = Car::create(m_setup);
			m_car->setPosition(m_origin.x + m_visibleSize.width - m_car->getContentSize().width,
				m_origin.y + m_visibleSize.height - m_car->getContentSize().height);
			m_car->SetEngine(&m_engine);
			this->addChild(m_car);

			if (m_selectedButton > -1)
			{
				buttons[m_selectedButton]->setScale(1, 1);
			}
			buttons[index]->setScale(1.5, 1.5);

			m_selectedButton = index;

			break;
		}
		default:
			break;
		}
	}
};

