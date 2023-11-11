//
//  DatePicker.hpp
//  VN Stories
//
//  Created by AnhB Nguyen on 3/9/19.
//

#ifndef DatePicker_hpp
#define DatePicker_hpp

#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace ui;
USING_NS_CC;

class DatePicker: public Layout {
public:
     CREATE_FUNC(DatePicker);
   static DatePicker* showPicker();
private:
    
    void didLoadFromCSB();
    void onOk();
    
    ListView* date_scroll;
    ListView* month_scroll;
    ListView* year_scroll;
    
    bool is_ok = false;
    
};
CREATE_READER(DatePicker);
#endif /* DatePicker_hpp */
