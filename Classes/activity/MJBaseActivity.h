//
//  MJBaseActivity.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 9/21/17.
//

#ifndef MJBaseActivity_hpp
#define MJBaseActivity_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MJStoryInfoModel.h"
#include "Math_Libs.h"
#include "StorySubscribePopup.h"
#include "../base_include/MJDefault.h"

namespace ms {
    enum class AISpeakingMode {
        MIMIC,
        TALKING,
        DEFAULT
    };
    namespace delegate {
        class Activity {
        public:
            enum EndReason {
                FINISHED,
                INTERRUPTED,
                FREE_ACTION
            };
            virtual void onActivityEnd(cocos2d::Ref * sender, EndReason reason = FINISHED) = 0;
        };
    }
    
    class BaseActivity : public cocos2d::ui::Layout, public StorySubscribePopupDelegate {
    public:
		enum END_GAME_TYPE
		{
			EGT_AWESOME = 0,
			EGT_GOOD_JOB,
			EGT_HOORAY,
			EGT_WELL_DONE,
			EGT_CHAR,
			EGT_COMMON
		};

		static BaseActivity * createActivity(mj::model::ActInfo info, std::vector<math::resource::Image*> images,  mj::model::AISpeakingLessonInfo lessonInfo = mj::model::AISpeakingLessonInfo(), ms::AISpeakingMode mode = ms::AISpeakingMode::DEFAULT);
        void startActivity();
        void stopActivity();
        void pauseActivity();
        void resumeActivity();
		void commonEndGame(END_GAME_TYPE eType, std::string charGafPath = "", bool isNeedtextBonus = false,bool is_INTERRUPTED = false);
        void onEnter() override;
        
        CC_SYNTHESIZE(ms::delegate::Activity *, _activitydelegate, ActivityDelegate);
        CC_SYNTHESIZE_READONLY(mj::model::ActInfo, _info, Info);
        void setCloseButtonEnale(bool isEnable);
        void addCloseButton();
        std::function<void()> _callbackCloseActivity = nullptr;
    protected:
		struct CloseButtonData
		{
			bool onMove = false;
			bool onBot = false;
		};
		CloseButtonData close_button_data;
        bool game_play_enable = true;
        StorySubscribePopup* pauseGame(bool isPause = true, StorySubscribePopup::SUB_POPUP_TYPE type = StorySubscribePopup::SUB_POPUP_TYPE::SUB_LESSON);
        virtual void onCloseGame();
        void onCloseGameVmonkey(bool is_interrupted = false);

        bool _isEnableCloseButton =true;

    private:
        void onSubscribeOk() override;
        void onSubscribeCancel() override;
        void onSubscribeFree() override;
        void startParticle(int type, cocos2d::Node *parent);
    };
}

#endif /* MJBaseActivity_hpp */
