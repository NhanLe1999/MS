//
//  APAccountDefinesV3.cpp
//  MJStory
//
//  Created by Hieu Nguyen on 7/31/20.
//

#include "APAccountDefinesV3.h"
#include "APDatabase.h"
#include "APEnterEmailPopup.h"
#include "MMFirebaseDatabase.h"
#include "GettingUserInfoManager.h"
#include "Key_Default_Model.h"

void APAccountBaseV3::checkShowingEnterEmailPopup(Node * parent)
{
    auto user_info = ap::Database::getInstance().getUserInfo();
    
    // nếu chưa có email
    if (user_info._mail.empty() || DEF_DEBUG_V3_ALWAYS_SHOW_ENTER_EMAIL)
    {
        bool will_show_popup_enter_info = false;
        
        std::string str_last_time = MJDEFAULT->getStringForKey(key_last_time_enter_info + std::to_string(user_info._id), "0");
        
        time_t last_time = str_last_time.empty()? time_t(0):std::stol(str_last_time);
        
        if (last_time == 0)
        {
            will_show_popup_enter_info = true;
        }
        else
        {
            time_t current_time = std::time(nullptr);
            
            double diff_time = std::difftime(current_time, last_time);
            
            if (diff_time > (3*24*60*60))
            {
                int skip_counter = GettingUserInfoManager::GetInstance()->getRemindingUpdateInfoPopupCount();
                
                if (skip_counter < DEF_ENTER_INFO_MAX_COUNT)
                {
                    will_show_popup_enter_info = true;
                }
            }
        }
        
        if ( !MJDEFAULT->getBoolForKey(key_is_submit, false) && (will_show_popup_enter_info || DEF_DEBUG_V3_ALWAYS_SHOW_ENTER_EMAIL))
        {
            MJDEFAULT->setStringForKey(key_last_time_enter_info + std::to_string(user_info._id), std::to_string(std::time(nullptr)));
            
            auto popup_enteremail = APEnterEmailPopup::createPopup();
            popup_enteremail->showPopup();
            
            if (parent)
            {
                parent->addChild(popup_enteremail, INT_MAX-1);
            }
            else
            {
                Director::getInstance()->getRunningScene()->addChild(popup_enteremail, INT_MAX-1);
            }
        }
    }
}

void APAccountBaseV3::moveUpForm()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

   if (m_isFormMovedUp == false)
   {
       auto layout = utils::findChild<ui::Layout *>(this, "layout");
       
       if (layout)
       {
           m_isFormMovedUp = true;
           layout->runAction(MoveBy::create(0.25f, Point(0, m_moveUpDistance)));
       }
   }
#endif

}

void APAccountBaseV3::moveDownForm()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

   if (m_isFormMovedUp)
   {
       auto layout = utils::findChild<ui::Layout *>(this, "layout");
       
       if (layout)
       {
           m_isFormMovedUp = false;
           layout->runAction(MoveBy::create(0.25f, Point(0, -m_moveUpDistance)));
       }
   }
#endif
}

void APAccountBaseV3::effectShowing()
{
    this->setOpacity(0);
    this->runAction(FadeIn::create(0.25f));
}
