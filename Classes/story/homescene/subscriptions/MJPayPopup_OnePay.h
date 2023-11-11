//
//  MJPayPopup_OnePay.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#ifndef MJPayPopup_OnePay_hpp
#define MJPayPopup_OnePay_hpp

#include "MJPaySubPopup.h"
#include "StoryAlert.h"

class MJPayPopup_OnePay : public MJPaySubPopup, public StoryAlertDelegate {
public:
    CREATE_FUNC(MJPayPopup_OnePay);
    static MJPayPopup_OnePay * createPopup(const mj::ListPackages & packages, const std::string & product_id, const int & discount);

private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void didLoadFromCSB(mj::ListPackages packages, std::string product_id, int discount) override;
    void onNext(cocos2d::Ref * sender) override;

    void onAlertOK(cocos2d::Ref * sender) override;
};

CREATE_READER_CLASS(MJPayPopup_OnePayReader, MJPayPopup_OnePay);

#endif /* MJPayPopup_OnePay_hpp */
