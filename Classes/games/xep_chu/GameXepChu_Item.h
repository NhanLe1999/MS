
#ifndef GAME_XEP_CHU_ITEM_HPP
#define GAME_XEP_CHU_ITEM_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"

USING_NS_CC;
using namespace ui;

enum ITEM_WORD_STATE
{
	IWS_NONE = -1,
	IWS_IDLE,
	IWS_HOLDING,
	IWS_TRUE_PLACE
};

class GameXepChu_Item : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(GameXepChu_Item);

    struct ItemInfo {
		bool fly = false;
        std::string text;
		std::string img_path;
        std::string audio_path;
		float time_call;
		float time_end;
    };
    
    static GameXepChu_Item * createItem(ItemInfo item_info,Color4B aColor);
    ItemInfo _item_info;
	int line = 0;
	cocos2d::ui::Text* text = nullptr;
	float rotate_info = 0;
	cocos2d::Vec2 posFlying = Vec2(0, 0);
	ITEM_WORD_STATE item_state = ITEM_WORD_STATE::IWS_NONE;
	Color4B curColor;
	
	float timeToJump = 0;
	float timeRandJump = 0;
	void update(float delta);
    void onEnter() override;
	void setTextBlank();
	void setFlyType();

	void onHold();
	void scaleHold();
	void onShowImage();
	void playGiatGiat();

	void onFinishAudio(bool isFinish);

	void setCallback(const std::function<void()> afunc);
	void setTextColor(Color4B color);
	void runActionText(cocos2d::Vec2 post);
	std::string getTextItem();
private:
	void didLoadFromCSB(ItemInfo item_info, Color4B aColor);
	std::function<void()> aCallbackFunc = nullptr;
};

CREATE_READER(GameXepChu_Item);

#endif
