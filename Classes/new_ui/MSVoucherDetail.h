//
//  MSVoucherDetail.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 3/11/19.
//

#ifndef MSVoucherDetail_hpp
#define MSVoucherDetail_hpp

#include "cocostudiomacro2.h"

class MSVoucherDetail : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol {
public:
    CREATE_FUNC(MSVoucherDetail);
    static MSVoucherDetail * createView();
    
private:
    void didLoadFromCSB();
    void onEnter() override;
    void onExit() override;
    
    void onClose(cocos2d::Ref * sender);
    
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
};

CREATE_READER(MSVoucherDetail);

#endif /* MSVoucherDetail_hpp */
