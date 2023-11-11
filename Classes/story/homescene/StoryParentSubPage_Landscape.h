//
//  StoryParentSubPage_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/13/17.
//
//

#ifndef StoryParentSubPage_Landscape_hpp
#define StoryParentSubPage_Landscape_hpp

#include "CocoStudioMacro.h"
using namespace cocos2d;
using namespace ui;
using namespace std;
enum StoryParentOptionTag {
    SPO_NOTIFICATION = 101,
    SPO_LANGUAGES = 201,
    SPO_LICENSE = 202,
    SPO_COUPON = 203,
    SPO_DELETE_CACHE = 204,
    SPO_MANAGER_DEVICE = 205,
    SPO_MANAGE_LICENSES = 215,
    SPO_MUSIC_SETTING = 206,
	SPO_ACCOUNT_PROFILE = 216,
    SPO_READINGLEVEL = 301,
    SPO_SUPPORT = 302,
    SPO_TERMS_OF_USE = 303,
    SPO_PRIVACY_POLICY = 304,
    SPO_DEVICE_ID = 305,
    SPO_DEBUG = 999,
    DELETE_ACCOUNT = 333
};


class StoryParentSubPage_Landscape : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:

    static void initReader();

    static StoryParentSubPage_Landscape * createPage(StoryParentOptionTag tag);
    void adjustWidth(float width);

protected:
    virtual void didLoadFromCSB();
};


#endif /* StoryParentSubPage_Landscape_hpp */
