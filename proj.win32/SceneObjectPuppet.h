#pragma once

#include "cocos2d.h"
#include "Pointer.h"
#include "PhysicsEngine.h"

template <class T, class Puppeteer>
class SceneObjectPuppet : public cocos2d::Sprite
{
public:
	void SetEngine(PhysicsEngine* engine)
	{
		m_engine = engine;
	}

	void onEnter() override
	{
		if (m_engine)
		{
			if (m_puppeteer)
			{
				onExit();
			}

			m_puppeteer = CreatePuppeteer(m_engine);
		}
	}

	void onExit() override
	{
		m_puppeteer.Clear();
	}

protected:
	virtual Pointer<Puppeteer> CreatePuppeteer(PhysicsEngine*) = 0;

	static T* createFromFile(const char* fileName)
	{
		T* sprite = new (std::nothrow) T();
		if (sprite && sprite->initWithFile(fileName))
		{
			sprite->autorelease();
			return sprite;
		}

		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

private:
	Pointer<Puppeteer> m_puppeteer = nullptr;
	PhysicsEngine* m_engine = nullptr;
};
