//
//  MSPlaylist_Player.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/28/18.
//

#ifndef MSPlaylist_Player_hpp
#define MSPlaylist_Player_hpp

#include "cocostudiomacro2.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
namespace mj { namespace model {
    struct AudiobookInfo;
}}

class MSPlaylist_Player : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(MSPlaylist_Player);
    static MSPlaylist_Player * createView();
    
private:
    void didLoadFromCSB();
    void onEnter() override;
    
    void onDown(cocos2d::Ref * sender);
    void onSelectItem(cocos2d::Ref * sender);
    
    std::vector<mj::model::AudiobookInfo> _audiobooks;
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MSPlaylist_Player);

#endif

#endif /* MSPlaylist_Player_hpp */

