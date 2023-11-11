#ifndef __DuaNgu_IOS__TruyenTranhPage__
#define __DuaNgu_IOS__TruyenTranhPage__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "NhanVat.h"
#include "TextStory.h"
#include "NhanVatObj.h"
#include "NVLayout.h"
#include "MSPageImp.h"

#define kTagGafPlay 2

class MSStoryGuide;

using namespace cocos2d;
using namespace ui;
class TruyenTranhPage : public Layout, public cocostudio::WidgetCallBackHandlerProtocol, public MSPageImp
{
public:
    CREATE_FUNC(TruyenTranhPage);
    static TruyenTranhPage * createPage(int story_id, int lang_id, int page_id);
    
    void enableInteraction() override;
    void disableInteraction() override;
    void startPlaying() override;
    bool hasNextBoxText() override;
    bool hasPrevBoxText() override;
    int getPageIndex() override;
    float getStandardTime() override;
    int getGAFClickedNum() override;
    int getIMGClickedNum() override;
private:
    void preload(int story_id, int lang_id, int page_id);
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name);
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    void onTouchBegan();
    void onTouchEnded();
    void onTouchMoved();

    bool isShow;

    
    void onBackPage(cocos2d::Ref *sender);
    void onRefresh(cocos2d::Ref *sender);
    
    std::vector<NhanVat *> _nvEffect;
    std::vector<NhanVat *> _nvLayout;
    std::vector<NhanVat *> _nvGAFPlay;
    std::vector<NhanVat *> _nvGAFEffect;
    std::vector<NhanVat *> _nvGAF_NoAutoPlay;
    
    
    TextStory *textStory;
    void onShow(cocos2d::Ref * sender);
    ValueMap mapPage;
    cocos2d::ui::Layout *nhanvat_layout;
    cocos2d::ui::ImageView *bgPage;
    //load trang truyen
    void loadNhanvat();
    cocos2d::Point pointTouch;
    bool isHighlightDone = false;
    int story_type = 1;
    
    void onExit();
    void display_cheat_playAllGaf();
    void play_cheat_allgaf(Ref *sender);
    
    //gaf_order
    void dumpGafOrderToMap();
    
    bool isCloseClicked = false;
    int _pageid = 0, _storyid = 0, _langid = 0;
    
public:
    void onNextPage(cocos2d::Ref *sender);
    void onNextEffectNV();
    void onStartEffectNV();
    bool _enable_touch = false, _enable_first = false;
    void onPlayAllGAF();
    int num_gaf_object = 0;
    int count_gaf_runfirst = 0, count_gaf_play = 0, count_gaf_done = 0;
    void pauseStarGuide();
    void resumStarGuide();
    void displayTextNV(std::string _content, float _wtext, Vec2 _posText, float _duration = 1);
    void displayEffectTextMC(std::string _str_object, float duration);
    void playGAFWithOrder();
    void playGAFOrderCallback();
    void playGAFWhenMCDone();
    
    MSStoryGuide * _guide;
    
    //gaf_order
    std::vector<int> list_gaforder_index, list_gaforder_number;
    cocos2d::ValueMap map_gaf_orderindex;
    int count_gafplay_done = 0;
    int index_list_gaf = 0, count_gafonturn = 0;

    void stopAllActionAndEffect();
    bool isreplacescene_enable = false, isEndStory = false;
    void onKeyboardPage(int index_keyboard);
    void onPauseAllAnimationAndSound();
    void fixSizeDisplay();
};

CREATE_READER_CLASS(TruyenTranhPageReader, TruyenTranhPage);

#endif /* defined(__ALTP__TruyenTranhPage__) */
