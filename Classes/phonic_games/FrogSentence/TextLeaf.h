#include "cocos2d.h"

USING_NS_CC;

class TextLeaf : public Sprite
{
public:
	static TextLeaf * create(const std::string& filename, const std::string& text);
	bool initWithText(const std::string & filename, const std::string& text);
	virtual void onEnter() override;
	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);
	void setTouchEndedCallback(std::function<void(TextLeaf*)> callback);
	void setTouchBeganCallback(std::function<void(TextLeaf*)> callback);
	void setBasePosition(cocos2d::Vec2 pos);
	void disableClick();
	cocos2d::Vec2 getBasePosition();
	std::string getText();
	cocos2d::Label* getLabel();
	void setLabel(Label* label);
protected:
	TextLeaf();
	cocos2d::Vec2 m_lastLocation;
	std::function<void(TextLeaf*)> m_touchEndedCallback;
	std::function<void(TextLeaf*)> m_touchBeganCallback;
	std::string m_text;
	cocos2d::Vec2 m_basePosition;
	cocos2d::Label* _label;
	bool m_disableClick;
private:
};
