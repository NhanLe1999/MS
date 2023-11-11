#pragma once
#include "GAFWrapper.h"
#include "GameData.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
using namespace cocos2d;

namespace WhackAMole {
	class Mole;

	class HitMoleListener {
	public:
		virtual void onMoleHit(float time, void* mole) = 0;
		virtual void onMoleHide(); // time out
	};

	class Mole : public GAFWrapper
	{
	public:
		static Mole* createMole(const std::string &gaf_name);
		bool isUp();
		void rise();
        void hide();
		virtual void onEnter() override;
		virtual void onExit() override;
		void setOnHitListener(HitMoleListener* listener);
		bool init(const std::string & gaf_name);
		bool onTouchBegan(Touch* touch, Event* event);
		virtual void update(float dt) override;
		void setText(mj::GameObject str);
		void reset();
        void setDown(bool down);
	protected:
		Mole();
		bool m_isUp;
        bool m_isDown = false;
		HitMoleListener* m_onHitListener;
		cocos2d::Label* m_label;
		cocos2d::Vec2 m_baseLabelPos;
		cocos2d::Vec2 m_labelOffset;
		mj::GameObject m_go;
		bool m_invulnerable; // avoid repeated hit 
		float m_hitTime;
	private:
		void hit();
	};
}
