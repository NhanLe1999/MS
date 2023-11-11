#pragma once

#include "cocos2d.h"
#include "GAFWrapper.h"
#include "GameData.h"
//#include "GAFAsset.h"

namespace WizardCauldron_VM {
	class TextNode_VM : public cocos2d::Node {
	public:
		static TextNode_VM* create(gaf::GAFAsset* asset);
		CREATE_FUNC(TextNode_VM);
		void setText(mj::GameObject text);
		virtual bool init() override;
		virtual void onEnter();
		cocos2d::Color4B getColor();
		mj::GameObject getString();
		bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unusedEvent);
		void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unusedEvent);
		void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unusedEvent);
		void setTouchReleaseCallback(std::function<bool(TextNode_VM*)> callback);
		void setTouchStartCallback(std::function<void(TextNode_VM*)> callback);
		void setEffectEnabled(bool isEnabled);
		void setBasePosition(cocos2d::Vec2 position);
		cocos2d::Vec2 getBasePosition();
		void moveToBasePosition();
	protected:
		TextNode_VM();
		void initColor(int index = -1); // texture name
		cocos2d::Label* m_label;
	private:
		std::map<int, cocos2d::Color4B> m_colorMap;
		cocos2d::Color4B m_color;
		cocos2d::Node* m_bg;
		cocos2d::Vec2 m_lastTouchLocation;
		std::function<bool(TextNode_VM*)> m_touchReleaseCallback;
		std::function<void(TextNode_VM*)> m_touchStartCallback;
		cocos2d::Vec2 m_basePosition;
		GAFWrapper* m_effect;
		mj::GameObject m_text;
		gaf::GAFAsset* m_gafAsset;
		bool m_touchEnabled;
	};
}
