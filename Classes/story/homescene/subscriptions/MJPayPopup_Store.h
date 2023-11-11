//
//  MJPayPopup_Store.hpp
//  Azu7
//
//  Created by To Ngoc Linh on 5/31/17.
//
//

#ifndef MJPayPopup_Store_hpp
#define MJPayPopup_Store_hpp

#include "CocoStudioMacro.h"
#include "MJPaySubPopup.h"

class MJPayPopup_Store : public MJPaySubPopup {
public:
    CREATE_FUNC(MJPayPopup_Store);
    static MJPayPopup_Store * createPopup(const mj::ListPackages & packages, const std::string & product_id, const int & discount);

private:
    void didLoadFromCSB(mj::ListPackages packages, std::string product_id, int _discount) override;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    void onNext(cocos2d::Ref * sender) override;
};

CREATE_READER_CLASS(MJPayPopup_StoreReader, MJPayPopup_Store);

#endif /* MJPayPopup_Store_hpp */
