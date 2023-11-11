//
//  MSNotificationPopup.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/26/18.
//

#ifndef MSNotificationPopup_hpp
#define MSNotificationPopup_hpp

#include "cocostudiomacro2.h"
#include "MJStoryInfoModel.h"

class MSNotificationPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSNotificationPopup);
    static MSNotificationPopup * createPopup(mj::model::NotificationInfo info);
    static void show(mj::model::NotificationInfo info);
private:
    void didLoadFromCSB(mj::model::NotificationInfo info);
    void onEnter() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    mj::model::NotificationInfo _info;
    void onClose(cocos2d::Ref * sender);
    void onOpen(cocos2d::Ref * sender);
};

CREATE_READER(MSNotificationPopup);

#endif /* MSNotificationPopup_hpp */
