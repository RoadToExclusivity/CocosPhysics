#include "CarSetupScene.h"
#include "MainScene.h"
#include "CarPuppet.h"
#include "GroundPuppet.h"
#include "GameOverScene.h"

USING_NS_CC;
using namespace ui;

const cocos2d::Color3B CarSetupScene::BTN_GO_TITLE_COLOR = Color3B(0, 153, 0);
const cocos2d::Color4B CarSetupScene::BACKGROUND_GRADIENT_FROM = Color4B(51, 51, 102, 255);
const cocos2d::Color4B CarSetupScene::BACKGROUND_GRADIENT_TO = Color4B(153, 153, 204, 255);
const cocos2d::Color3B CarSetupScene::LABEL_NORMAL = Color3B(255, 255, 255);
const cocos2d::Color3B CarSetupScene::LABEL_SELECTED = Color3B(204, 153, 51);
const float CarSetupScene::BTN_GO_SCALE = 0.25f;
const float CarSetupScene::BTNS_MIN_SCALE = 1;
const float CarSetupScene::BTNS_MAX_SCALE = 1.5f;
const float CarSetupScene::LABEL_MIN_SCALE = 1;
const float CarSetupScene::LABEL_MAX_SCALE = 1.1f;

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

	if (buttons)
	{
		delete[] buttons;
	}

	if (labels)
	{
		delete[] labels;
	}
}

void CarSetupScene::update(float dt)
{
	m_engine.tick(dt);
}

void CarSetupScene::AddButtons()
{
	buttons = new Button*[3];
	if (!buttons)
	{
		return;
	}

	buttons[0] = Button::create("icon_setup_1.png");
	buttons[1] = Button::create("icon_setup_2.png");
	buttons[2] = Button::create("icon_setup_3.png");

	auto btnGo = Button::create("goButton.png");

	btnGo->setTitleText("GO!");
	btnGo->setTitleColor(BTN_GO_TITLE_COLOR);
	btnGo->setTitleFontSize(BTN_GO_TITLE_FONT_SIZE);
	btnGo->addTouchEventListener(CC_CALLBACK_2(CarSetupScene::onContinue, this));

	buttons[0]->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		CarSettingEvent(type, WheelSetup::SMALL_WHEELS, ClearanceSetup::LOW_CLEARANCE, 0);
	});

	buttons[1]->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		CarSettingEvent(type, WheelSetup::BIG_WHEELS, ClearanceSetup::LOW_CLEARANCE, 1);
	});

	buttons[2]->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type)
	{
		CarSettingEvent(type, WheelSetup::BIG_WHEELS, ClearanceSetup::HIGH_CLEARANCE, 2);
	});

	btnGo->setScale(BTN_GO_SCALE);
	btnGo->setPosition(Vec2(m_origin.x + m_visibleSize.width - BTN_GO_OFFSET_X, m_origin.y + BTN_GO_OFFSET_Y));
	
	float btnWidth = buttons[0]->getCustomSize().width;
	for (size_t i = 0; i < 3; ++i)
	{
		buttons[i]->setPosition(Vec2(m_origin.x + BTNS_FIRST_OFFSET_X + BTNS_MAX_SCALE * btnWidth / 2.0f +
									BTNS_MAX_SCALE * i * (btnWidth + BTNS_MIN_DISTANCE), m_origin.y + BTNS_OFFSET_Y));
		this->addChild(buttons[i]);
	}
	this->addChild(btnGo);
}

void CarSetupScene::AddDescription()
{
	labels = new Label*[3];
	if (!labels)
	{
		return;
	}

	labels[0] = Label::createWithSystemFont("Small wheels\nLow clearance\n\n\
											High speed\nLow control\n", "Arial", LABELS_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);
	labels[1] = Label::createWithSystemFont("Big wheels\nLow clearance\n\n\
											Medium speed\nMedium control\n", "Arial", LABELS_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);
	labels[2] = Label::createWithSystemFont("Big wheels\nHigh clearance\n\n\
											Low speed\nHigh control\n", "Arial", LABELS_FONT_SIZE, Size::ZERO, TextHAlignment::CENTER);

	for (size_t i = 0; i < 3; ++i)
	{
		labels[i]->setPosition(buttons[i]->getPositionX(), m_visibleSize.height / 2.0 + LABELS_OFFSET_Y);
		this->addChild(labels[i]);
	}
}

void CarSetupScene::AddGround()
{
	auto ground = Ground::create(&m_engine);
	if (!ground)
	{
		return;
	}

	ground->setPosition(m_origin.x + m_visibleSize.width - GROUND_OFFSET_X_RIGHT,
		m_origin.y + m_visibleSize.height - GROUND_OFFSET_Y_TOP);

	this->addChild(ground);
}

bool CarSetupScene::init()
{
	if (!LayerGradient::initWithColor(BACKGROUND_GRADIENT_FROM, BACKGROUND_GRADIENT_TO))
	{
		return false;
	}
	
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_origin = Director::getInstance()->getVisibleOrigin();

	AddButtons();
	AddDescription();
	AddGround();
	EmulateButtonTouch(SELECTED_BTN_INDEX);

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

void CarSetupScene::CarSettingEvent(Widget::TouchEventType type, WheelSetup wheel, ClearanceSetup clearance, size_t index)
{
	switch (type)
	{
	case Widget::TouchEventType::ENDED:
	{
		m_setup.SetWheel(wheel);
		m_setup.SetClearance(clearance);

		if (m_car)
		{
			this->removeChild(m_car);
		}

		m_car = Car::create(m_setup, &m_engine);
		m_car->setPosition(m_origin.x + m_visibleSize.width - CAR_OFFSET_X_RIGHT,
			m_origin.y + m_visibleSize.height - CAR_OFFSET_Y_TOP);
		this->addChild(m_car);

		if (m_selectedButton > -1)
		{
			buttons[m_selectedButton]->setScale(BTNS_MIN_SCALE);
			labels[m_selectedButton]->setColor(LABEL_NORMAL);
			labels[m_selectedButton]->setScale(LABEL_MIN_SCALE);
		}
		buttons[index]->setScale(BTNS_MAX_SCALE);
		labels[index]->setColor(LABEL_SELECTED);
		labels[index]->setScale(LABEL_MAX_SCALE);

		m_selectedButton = index;

		break;
	}
	default:
		break;
	}
}

void CarSetupScene::EmulateButtonTouch(size_t index) const
{
	if (!buttons || !buttons[index])
	{
		return;
	}

	Touch* t = new Touch();
	bool wasHighlighted = buttons[index]->isHighlighted();
	buttons[index]->setHighlighted(true);
	buttons[index]->onTouchEnded(t, nullptr);
	buttons[index]->setHighlighted(wasHighlighted);
	delete t;
}