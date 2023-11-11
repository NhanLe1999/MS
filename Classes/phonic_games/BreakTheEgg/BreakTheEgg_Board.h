
#ifndef BreakTheEgg_Board_HPP
#define BreakTheEgg_Board_HPP

#include "cocostudiomacro2.h"
#include "GAFWrapper.h"
#include "PhonicInfoModel.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
#include "ui/UIVideoPlayer.h"
using namespace cocos2d::experimental::ui;
#endif
USING_NS_CC;
using namespace cocos2d;
using namespace ui;

class BreakTheEgg_BoardDelegate {
public:
    virtual void onBoardStartHide() {};
};

class BreakTheEgg_Board : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(BreakTheEgg_Board);
    static BreakTheEgg_Board * createItem(mj::PHONIC_BOARD_INFO item_info);
    void setDelegate(BreakTheEgg_BoardDelegate *delegate);
    void showBoard();
private:
    void onEnter() override;
    
	void didLoadFromCSB(mj::PHONIC_BOARD_INFO item_info);
    mj::PHONIC_BOARD_INFO _item_info;
    void showVideo(std::string video_path, cocos2d::Size video_size, Vec2 video_position, cocos2d::Node *parent);
    void showImage(std::string img_path, Vec2 img_position);
    
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM != CC_PLATFORM_MAC)
    void videoEventCallback(Ref* sender, VideoPlayer::EventType eventType);
    VideoPlayer* videoPlayer = nullptr;
#endif
    
    BreakTheEgg_BoardDelegate *_delegate;
    std::string font_name = "fonts/phonicfont.ttf";
    
    cocos2d::Size screen_size;
    cocos2d::ui::ImageView *board;
};

CREATE_READER(BreakTheEgg_Board);

#endif
