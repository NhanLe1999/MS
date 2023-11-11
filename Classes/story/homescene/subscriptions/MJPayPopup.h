//
//  MJPayPopup.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#ifndef MJPayPopup_hpp
#define MJPayPopup_hpp

#include <stdio.h>
#include "CocoStudioMacro.h"
#include "MJInAppInfo.h"

class MJPayPopup : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MJPayPopup);
    static MJPayPopup * createPopUp(const mj::ListPackages & packages, const std::string & product_id, const int & discount);

private:
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name);

    void onOnePay(cocos2d::Ref * sender);
    void onCOD(cocos2d::Ref * sender);
    void onStore(cocos2d::Ref * sender);
    void onBankTransfer(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);

    std::string _product_id;
    int _discount;
    mj::ListPackages _packages;
};

CREATE_READER_CLASS(MJPayPopupReader, MJPayPopup);

#endif /* MJPayPopup_hpp */
