#include "CarSetupScene.h"
#include "MainScene.h"
#include "CarPuppet.h"
#include "GroundPuppet.h"

USING_NS_CC;
using namespace ui;

CarSetupScene::CarSetupScene()
	:m_setup(), m_engine(this, ENGINE_PTM)
{
}

Scene* CarSetupScene::createScene()
{
	auto scene = Scene::create();
	auto layer = CarSetupScene::create();

	scene->addChild(layer);

	return scene;
}

void CarSetupScene::onEnter()
{
	LayerGradient::onEnter();
	scheduleUpdate();
}

void CarSetupScene::onExit()
{
	unscheduleUpdate();
	LayerGradient::onExit();
}

void CarSetupScene::update(float dt)
{
	m_engine.tick(dt);
}

bool CarSetupScene::init()
{
	if (!LayerGradient::initWithColor(Color4B(51, 51, 102, 255), Color4B(153, 153, 204, 255)))
	{
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	m_visibleSize = visibleSize;
	m_origin = origin;

	m_car = Car::create(m_setup);
	m_car->setPosition(origin.x + visibleSize.width - m_car->getContentSize().width, 
						origin.y + visibleSize.height - m_car->getContentSize().height);
	m_car->SetEngine(&m_engine);

	auto ground = Ground::create();
	ground->SetEngine(&m_engine);
	ground->setPosition(origin.x + visibleSize.width - ground->getContentSize().width / 2.0 - 40, 
						origin.y + visibleSize.height / 2.0 + 50);

	this->addChild(m_car);
	this->addChild(ground);

	buttons = new Button*[3];
	buttons[0] = Button::create("icon_small_wheels_low_clearance.png");
	buttons[1] = Button::create("icon_big_wheels_low_clearance.png");
	buttons[2] = Button::create("icon_big_wheel_high_clearance.png");
	//buttons = newButtons;

	Button* btnGo = Button::create("goButton.png");

	btnGo->setTitleText("GO!");
	btnGo->setTitleColor(Color3B(0, 153, 0));
	btnGo->setTitleFontSize(80);
	btnGo->addTouchEventListener(CC_CALLBACK_2(CarSetupScene::onContinue, this));

	buttons[0]->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		CarSettingEvent<WheelSetup::SMALL_WHEELS, ClearanceSetup::LOW_CLEARANCE, 0>(sender, type);
	});

	buttons[1]->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		CarSettingEvent<WheelSetup::BIG_WHEELS, ClearanceSetup::LOW_CLEARANCE, 1>(sender, type);
	});

	buttons[2]->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		CarSettingEvent<WheelSetup::BIG_WHEELS, ClearanceSetup::HIGH_CLEARANCE, 2>(sender, type);
	});

	/*buttons[0]->setTitleText("Low clearance, small wheels");
	//buttons[0]->addTouchEventListener([&, visibleSize, origin](Ref* sender, Widget::TouchEventType type)
	//{
	//	switch (type)
	//	{
	//	case Widget::TouchEventType::ENDED:
	//	{
	//		m_setup.SetWheel(WheelSetup::SMALL_WHEELS);
	//		m_setup.SetClearance(ClearanceSetup::LOW_CLEARANCE);

	//		this->removeChild(m_car);
	//		
	//		m_car = Car::create(m_setup);
	//		m_car->setPosition(origin.x + visibleSize.width - m_car->getContentSize().width, 
	//							origin.y + visibleSize.height - m_car->getContentSize().height);
	//		m_car->SetEngine(&m_engine);
	//		this->addChild(m_car);

	//		if (m_selectedButton > -1)
	//		{
	//			buttons[m_selectedButton]->setScale(1, 1);
	//		}

	//		buttons[0]->setScale(1.5, 1.5);

	//		m_selectedButton = 0;

	//		break;
	//	}
	//	default:
	//		break;
	//	}
	//});

	////buttons[1]->setTitleText("Low clearance, big wheels");
	//buttons[1]->addTouchEventListener([&, visibleSize, origin, this](Ref* sender, Widget::TouchEventType type)
	//{
	//	switch (type)
	//	{
	//	case Widget::TouchEventType::ENDED:
	//	{
	//		m_setup.SetWheel(WheelSetup::BIG_WHEELS);
	//		m_setup.SetClearance(ClearanceSetup::LOW_CLEARANCE);
	//		
	//		this->removeChild(m_car);

	//		m_car = Car::create(m_setup);
	//		m_car->setPosition(origin.x + visibleSize.width - m_car->getContentSize().width,
	//			origin.y + visibleSize.height - m_car->getContentSize().height);
	//		m_car->SetEngine(&m_engine);
	//		this->addChild(m_car);
	//		
	//		if (m_selectedButton > -1)
	//		{
	//			this->buttons[m_selectedButton]->setScale(1, 1);
	//		}
	//		buttons[1]->setScale(1.5, 1.5);

	//		m_selectedButton = 1;

	//		break;
	//	}
	//	default:
	//		break;
	//	}
	//});

	////buttons[2]->setTitleText("High clearance, big wheels");
	//buttons[2]->addTouchEventListener([&, visibleSize, origin](Ref* sender, Widget::TouchEventType type)
	//{
	//	switch (type)
	//	{
	//	case Widget::TouchEventType::ENDED:
	//	{
	//		m_setup.SetWheel(WheelSetup::BIG_WHEELS);
	//		m_setup.SetClearance(ClearanceSetup::HIGH_CLEARANCE);
	//		
	//		this->removeChild(m_car);

	//		m_car = Car::create(m_setup);
	//		m_car->setPosition(origin.x + visibleSize.width - m_car->getContentSize().width,
	//			origin.y + visibleSize.height - m_car->getContentSize().height);
	//		m_car->SetEngine(&m_engine);
	//		this->addChild(m_car);

	//		if (m_selectedButton > -1)
	//		{
	//			buttons[m_selectedButton]->setScale(1, 1);
	//		}
	//		((Button*)sender)->setScale(1.5, 1.5);

	//		m_selectedButton = 2;

	//		break;
	//	}
	//	default:
	//		break;
	//	}
	//});*/
	
	btnGo->setScale(1.0 / 4);
	btnGo->setPosition(Vec2(origin.x + visibleSize.width - btnGo->getContentSize().width / 8.0 - 20, origin.y + 100));
	
	for (size_t i = 0; i < 3; ++i)
	{
		//auto enlarge = ScaleBy::create(2, 1.25), reduce = ScaleBy::create(2, 0.8);
		//auto seq = Sequence::create(enlarge, reduce, nullptr);
		//buttons[i]->runAction(RepeatForever::create(seq));

		buttons[i]->setPosition(Vec2(origin.x + 20 + buttons[i]->getCustomSize().width / 2.0 + 
									1.5 * i * (buttons[i]->getCustomSize().width + 20), origin.y + visibleSize.height / 2.0));
		this->addChild(buttons[i]);
	}
	this->addChild(btnGo);

	return true;
}

void CarSetupScene::onContinue(Ref* sender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
	{
		auto mainScene = MainScene::createScene(m_setup);
		Director::getInstance()->replaceScene(mainScene);
		break;
	}
	default:
		break;
	}
}