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
	
	void AddButtons();
	void AddDescription();
	void AddGround();

	cocos2d::ui::Button **buttons = nullptr;
	cocos2d::Label **labels = nullptr;
	Car* m_car = nullptr;
	PhysicsEngine m_engine;
	CarSetup m_setup;
	int m_selectedButton = -1;

	cocos2d::Vec2 m_origin;
	cocos2d::Size m_visibleSize;

	static const int ENGINE_PTM = 60;
	static const int BTN_GO_TITLE_FONT_SIZE = 80;
	static const int CAR_OFFSET_X_RIGHT = 140;
	static const int CAR_OFFSET_Y_TOP = 130;
	static const int BTN_GO_OFFSET_X = 110;
	static const int BTN_GO_OFFSET_Y = 100;
	static const int BTNS_FIRST_OFFSET_X = 10;
	static const int BTNS_OFFSET_Y = 100;
	static const int BTNS_MIN_DISTANCE = 0;
	static const int GROUND_OFFSET_X_RIGHT = 130;
	static const int GROUND_OFFSET_Y_TOP = 240;
	static const int LABELS_FONT_SIZE = 28;
	static const int LABELS_OFFSET_Y = 80;
	static const size_t SELECTED_BTN_INDEX = 0;

	static const float BTN_GO_SCALE;
	static const float BTNS_MIN_SCALE;
	static const float BTNS_MAX_SCALE;
	static const float LABEL_MIN_SCALE;
	static const float LABEL_MAX_SCALE;
	
	static const cocos2d::Color3B BTN_GO_TITLE_COLOR;
	static const cocos2d::Color4B BACKGROUND_GRADIENT_FROM;
	static const cocos2d::Color4B BACKGROUND_GRADIENT_TO;
	static const cocos2d::Color3B LABEL_NORMAL;
	static const cocos2d::Color3B LABEL_SELECTED;

	void CarSettingEvent(cocos2d::ui::Widget::TouchEventType type, WheelSetup wheel, ClearanceSetup clearance, size_t index);
	void EmulateButtonTouch(size_t index) const;
};

