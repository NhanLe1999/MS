//
//  MJPayPopup_BankTransfer.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#ifndef MJPayPopup_BankTransfer_hpp
#define MJPayPopup_BankTransfer_hpp

#include "CocoStudioMacro.h"
#include "MJPaySubPopup.h"

class MJPayPopup_BankTransfer : public MJPaySubPopup {
public:
    CREATE_FUNC(MJPayPopup_BankTransfer);
    static MJPayPopup_BankTransfer * createPopup(const mj::ListPackages & packages, const std::string & product_id, const int & discount);
    void didLoadFromCSB(mj::ListPackages packages, std::string product_id, int _discount) override;
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onNext(cocos2d::Ref * sender) override;
    
};

CREATE_READER_CLASS(MJPayPopup_BankTransferReader, MJPayPopup_BankTransfer);
#endif /* MJPayPopup_BankTransfer_hpp */
