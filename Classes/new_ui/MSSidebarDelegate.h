//
//  MSSidebarDelegate.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/7/17.
//

#ifndef MSSidebarDelegate_hpp
#define MSSidebarDelegate_hpp

#include <string>

namespace ms {
    enum HomeTab {
        STORIES,
        LESSONS,
        MATERIAL,           // phần Học liệu: main
        MATERIAL_STORIES,   // phần Học liệu: for Stories
        MATERIAL_PHONICS,   // phần Học liệu: for Phonics
        SUBSCRIPTIONS,
        FREE,
        SETTINGS,
        ACCOUNT,    // thông tin tài khoản người dùng, account: new_v3
        PROMOTION,
        AUDIOBOOKS,
        FAVOURITE,
        LICENSE,
        LESSONS_PHONIC,
        LESSONS_COMPREHENSION,
        AI_SPEAKING
    };
    
    enum MovingDirection {
        UP = 1,
        DOWN = -1,
        NONE = 0
    };
    
    class SidebarDelegate {
    public:
        virtual void onSelectedTab(HomeTab tab, MovingDirection direction) = 0;
        virtual void onSelectedLanguage(bool is_show, std::string lang_code) = 0;
    };
}

#endif /* MSSidebarDelegate_hpp */
