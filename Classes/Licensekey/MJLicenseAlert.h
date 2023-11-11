//
//  MJ_MJLicenseAlert.h
//  Created by HoiSung on 02/02/18.
//
//

#ifndef MJ_MJLicenseAlert_h
#define MJ_MJLicenseAlert_h

#include <stdio.h>
#include "cocostudiomacro2.h"

using namespace cocos2d;
using namespace std;
class MJLicenseAlertDelegate {
public:
    virtual void onMJAlertClicked(bool is_ok, int tag) {};
};

class MJLicenseAlert : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(MJLicenseAlert);
    static MJLicenseAlert *showAlert(int tag, std::string message, std::string ok_label, std::string cancel_label, MJLicenseAlertDelegate *delegate = nullptr, bool showExit = true, bool xanhdo = true);
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void didLoadFromCSB(int tag, string message, string ok_label, string cancel_label, MJLicenseAlertDelegate *delegate, bool showExit,bool xanhdo);
    void onEnter() override;
    MJLicenseAlertDelegate *delegate = nullptr;
    void onOkClicked(cocos2d::Ref *sender);
    void onCancelClicked(cocos2d::Ref *sender);
    int alert_tag = 0;
    void onHide(bool is_ok);
    bool is_showed = false;
    Sprite *background;
    void fixSizeText(Label *label, int font_size, float max_height);
};
CREATE_READER(MJLicenseAlert);
#endif /* MJ_MJLicenseAlert_h */
