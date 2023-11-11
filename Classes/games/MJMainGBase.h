//
//  MJMainGBase.h
//
//  Created by Ha Van Hoi on 3/9/16.
//
//

#ifndef __MJMainGBase__
#define __MJMainGBase__

#include "cocostudiomacro2.h"
#include "cocos2d.h"
#include "GAFWrapper.h"
#include "HSAudioEngine.h"
#include "PhonicInfoModel.h"
#include "PhonicConfig.h"
#include "MJBaseActivity.h"
#include "MJDefault.h"
#include "Math_Libs.h"

using namespace cocos2d;
using namespace std;

class MJMainGBaseDelegate {
public:
    virtual void getGameStatus(string sender) {};
};

class MJMainGBase : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(MJMainGBase);
    void onEnter() override;
    void onExit() override;
    virtual void onFinishGame();
    virtual void onFinishGameVMonkey();
    void startParticle(int type, cocos2d::Node *parent);
	void onExitTransitionDidStart() override;
    void showTapGuideGame(Vec2 pos, int loop = 100, float delay_time = 8.0f,std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
    void showDragGuideGame(Vec2 posStart, Vec2 posEnd, int loop = 100, float delay_time =0.5f, float dt = 4.5f,std::string gaf_guide_path = "gaf/guidegame/hand_tap/hand_tap.gaf", cocos2d::ui::Layout* parentLayout = NULL);
    void hideTapGuideGame();
    cocos2d::Size screen_size;
    int playBackgroundMusic(std::string file_path, float volume = 1.0f);
	void setConfigImages(std::vector<math::resource::Image*> images);
    bool is_auto_play = false;
    int number_turnplay = 0;

    void getGameSTT(string stt);
    void setDelegate(MJMainGBaseDelegate* delegate);
    static MJMainGBase* createActivity(mj::model::ActInfo info, std::string configPath, std::vector<math::resource::Image*> images);
    void onCloseGame(ms::delegate::Activity::EndReason reason = ms::delegate::Activity::EndReason::FINISHED);
protected:
	std::vector<math::resource::Image*> m_images;
    MJMainGBaseDelegate* _delegate;
    
    CC_SYNTHESIZE(ms::delegate::Activity *, _activitydelegate, ActivityDelegate);
};
#endif 
/* defined(__MJMainGBase__) */
