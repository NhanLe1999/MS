//
//  MSEventPopup.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 9/13/18.
//

#ifndef MSEventPopup_hpp
#define MSEventPopup_hpp

#include "cocos2d.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJDefault.h"
#include "MJHelper.h"


using namespace cocos2d;

struct EventInfo{
    std::string icon_path;
    std::string content_path;
    std::string url;
    Rect button_rect;
    int start_date;
    int end_date;
    bool is_image;
};

class MSEventPopup: public cocos2d::ui::Layout {
public:
    CREATE_FUNC(MSEventPopup);
    static bool show();
    static MSEventPopup* createEventPopup(EventInfo info);
    void updateEventInfo(std::string str);
private:
    void setupIcon(EventInfo info);
    void onEnter();
    void onExit();
    void onExitTransitionDidStar();
    void onIconClicked();
    void onEventClicked();
    void onCloseClicked();
    
    Size screen_size;
    
    EventInfo _info;
    Layout* trans_layout;
//    bool is_image = true;
//    bool in_event = true;
};

#endif /* MSEventPopup_hpp */
