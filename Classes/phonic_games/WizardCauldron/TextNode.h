#pragma once

#include "cocos2d.h"
#include "GAFWrapper.h"
#include "GameData.h"
//#include "GAFAsset.h"

namespace WizardCauldron {
	class TextNode : public cocos2d::Node {
	public:
		static TextNode* create(gaf::GAFAsset* asset, bool hideText = false);
		CREATE_FUNC(TextNode);
		void setText(mj::GameObject text);
		virtual bool init() override;
		virtual void onEnter();
		cocos2d::Color4B getColor();
		mj::GameObject getString();
		bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * unusedEvent);
		void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * unusedEvent);
		void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * unusedEvent);
		void setTouchReleaseCallback(std::function<bool(TextNode*)> callback);
		void setTouchStartCallback(std::function<void(TextNode*)> callback);
		void setEffectEnabled(bool isEnabled);
		void setBasePosition(cocos2d::Vec2 position);
		cocos2d::Vec2 getBasePosition();
		void moveToBasePosition();
	protected:
		TextNode();
		void initColor(int index = -1); // texture name
		cocos2d::Label* m_label;
	private:
		bool _hideText = false; 
		int  _minSizeHeight = 10000;
		int  _minSizeWidth = 10000;
		std::map<int, cocos2d::Color4B> m_colorMap;
		cocos2d::Color4B m_color;
		cocos2d::Sprite* m_bg;
		cocos2d::Vec2 m_lastTouchLocation;
		std::function<bool(TextNode*)> m_touchReleaseCallback;
		std::function<void(TextNode*)> m_touchStartCallback;
		cocos2d::Vec2 m_basePosition;
		GAFWrapper* m_effect;
		mj::GameObject m_text;
		gaf::GAFAsset* m_gafAsset;
		bool m_touchEnabled;
	};
}
