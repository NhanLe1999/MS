
#ifndef GingerBreadMan_Board_HPP
#define GingerBreadMan_Board_HPP

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

class GingerBreadMan_BoardDelegate {
public:
    virtual void onBoardStartHide() {};
};

class GingerBreadMan_Board : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(GingerBreadMan_Board);
    static GingerBreadMan_Board * createItem(mj::PHONIC_BOARD_INFO item_info);
    void setDelegate(GingerBreadMan_BoardDelegate *delegate);
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
    
    GingerBreadMan_BoardDelegate *_delegate;
    std::string font_name = "fonts/phonicfont.ttf";
    
    cocos2d::Size screen_size;
    cocos2d::ui::ImageView *board;
};

CREATE_READER(GingerBreadMan_Board);

#endif
