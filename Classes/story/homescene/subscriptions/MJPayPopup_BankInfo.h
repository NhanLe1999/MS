//
//  MJPayPopup_BankInfo.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#ifndef MJPayPopup_BankInfo_hpp
#define MJPayPopup_BankInfo_hpp

#include "MJPaySubPopup.h"
#include "MJRequest.h"

class MJPayPopup_BankInfo : public MJPaySubPopup, public mj::RequestDelegate {
public:
    CREATE_FUNC(MJPayPopup_BankInfo);
    static MJPayPopup_BankInfo * createPopup(const mj::ListPackages & packages, const std::string & product_id, const int & discount);

private:
    void onEnter() override;
    void onExit() override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    cocos2d::ui::Widget::ccWidgetEventCallback onLocateEventCallback(const std::string & callback_name) override;
    void onNext(cocos2d::Ref * sender) override;

    mj::Request * _request = nullptr;
    void onRequestSuccess(int tag, rapidjson::Value data) override;
    void onRequestFailed(int tag, int error_code, std::string error_string, rapidjson::Value data) override;

    void onTextFieldEvent(cocos2d::Ref * sender, int event);

    int _event_counter;
    void onBack(cocos2d::Ref * sender) override;
};

CREATE_READER_CLASS(MJPayPopup_BankInfoReader, MJPayPopup_BankInfo);
#endif /* MJPayPopup_BankInfo_hpp */
