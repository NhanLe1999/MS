//
//  MJPayPopup_COD.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#ifndef MJPayPopup_COD_hpp
#define MJPayPopup_COD_hpp

#include "MJPaySubPopup.h"
#include "MJRequest.h"

class MJPayPopup_COD : public MJPaySubPopup, public mj::RequestDelegate {
public:
    CREATE_FUNC(MJPayPopup_COD);
    static MJPayPopup_COD * createPopup(const mj::ListPackages & packages, const std::string & product_id, const int & discount);

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
};

CREATE_READER_CLASS(MJPayPopup_CODReader, MJPayPopup_COD);
#endif /* MJPayPopup_COD_hpp */
