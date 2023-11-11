//
// Created by To Ngoc Linh on 9/12/17.
//

#ifndef MJSTORY_MJPAYDETAILS_H
#define MJSTORY_MJPAYDETAILS_H

#include "cocostudiomacro2.h"
#include "MJInAppInfo.h"

class MJPayDetails : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    static MJPayDetails * createPopup(mj::InAppPackage package);
    CREATE_FUNC(MJPayDetails);
private:
    void onEnter() override;
    void didLoadFromCSB(mj::InAppPackage package);

    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    void onBack(cocos2d::Ref * sender);
    void onContinue(cocos2d::Ref * sender);

    mj::InAppPackage _package;
};

CREATE_READER(MJPayDetails);

#endif //MJSTORY_MJPAYDETAILS_H
