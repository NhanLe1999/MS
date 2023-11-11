//
//  StoryRateAction.hpp
//  mj_gamelib
//
//  Created by To Ngoc Linh on 12/2/17.
//

#ifndef StoryRateAction_hpp
#define StoryRateAction_hpp

#define key_rate_action "key_rate_action"
#define key_count_to_show "key_count_to_show"
#define key_time_to_show "key_time_to_show"

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "StoryRateConfirm.h"
#include "StoryRateQuestLoveApp.h"
namespace mjrate {
    class StoryRateAction : public cocos2d::ui::Layout, public StoryRateConfirmDelegate, public StoryRateQuestLoveAppDelegate {
    
    public:
        static bool shouldShowRate();
        static void updateActionCompleted();
        StoryRateAction() {};

        ~StoryRateAction() {};

        enum TYPE_RATE_ACTION {
            ALL_ACTION,
            RATE_ACTION,
            FEEDBACK_ACTION
        };
        
        static StoryRateAction * createPage(int type);
        void didLoadData(int type);
        
    private:
        int  action_type;
        void showRatePopup();
        void showFeedbackPopup();
        void showRateConfirm();
        void showQuesLoveAppPopup();
        void showRateBonus();
        void showThanks();
        void onEnter() override;
        void onRateConfirmOk() override;
        void onRateQuestLoveAppOk() override;
        void onRateQuestLoveAppCancel() override;
        
    };
}
#endif /* StoryRateAction_hpp */
