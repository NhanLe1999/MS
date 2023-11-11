//
//  MJSearchbar.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#ifndef MJSearchbar_hpp
#define MJSearchbar_hpp

#include "cocostudiomacro2.h"
#include "MSGradeBoxDelegate.h"

namespace ms {
    namespace delegate {
        class Searchbar;
    }
}

class MSSearchbar : public cocos2d::ui::Layout, public cocostudio::WidgetCallBackHandlerProtocol, public ms::delegate::GradeBox {
public:
    CREATE_FUNC(MSSearchbar);
    static MSSearchbar * createBar(int i_gradeID);
    
private:
    void didLoadFromCSB(int i_gradeID);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;
    
    void onSearch(cocos2d::Ref * sender);
    void onSelectGradeBox() override;
    void onDeselectGradeBox(int grade_id) override;
    
    CC_SYNTHESIZE(ms::delegate::Searchbar *, _delegate, Delegate);
};

CREATE_READER(MSSearchbar);

#endif /* MJSearchbar_hpp */
