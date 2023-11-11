
#ifndef MSBookPage_hpp
#define MSBookPage_hpp

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MSBookData.h"
#include "MSBookText.h"
#include "MSBookObject.h"

#include "MSPageImp.h"

class MSStoryGuide;

using namespace cocos2d;
using namespace ui;
class MSBookPage : public cocos2d::ui::Layout, public MSBookObjectDelegate, public MSBookTextDelegate, public cocostudio::WidgetCallBackHandlerProtocol, public MSPageImp {
public:
    CREATE_FUNC(MSBookPage);
    static MSBookPage * createPage(int story_id, int lang_id, int page_id);
    void generateData(rapidjson::Value value_data);
    void onPauseAllAnimationAndSound();
    
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
    void didLoadFromCSB(int story_id, int lang_id, int page_id);
    int _pageid = 0;
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onEnter() override;
    void onExitTransitionDidStart() override;
    mj::BOOK_DATA book_data;
    void addTextMC();
    void addObject();
    void addBackgroud();
    
    cocos2d::Mat4 _original_transform;
    bool _is_dragging = false;
    double _angle = 0.0f;
    void transformPageByAngle(double angle);
    void transformPageTurn(double distance);
    
    Layout *background_layout;
    std::vector<MSBookObject *> list_object_ani;
    std::vector<MSBookObject *> list_object_touch;
    
    void onTouchObject(std::string _content, float _wtext, Vec2 _posText, float _duration, mj::BOOK_IMAGE_INFO i_info) override ;
    void displayTextNV(std::string _content, float _wtext, Vec2 _posText, float _duration);
    
    void onDoneHighlightTextMC() override;
    void onActionGuideEffectObject(std::string action) override;
    //gaf_order
    std::vector<int> list_gaforder_index, list_gaforder_number;
    cocos2d::ValueMap map_gaf_orderindex;
    int count_gafplay_done = 0;
    int index_list_gaf = 0, count_gafonturn = 0;
    void dumpGafOrderToMap();
    void playGAFWithOrder();
    void playGAFOrderCallback();
    MSBookText *book_text;
    void onRunAniObjectWithMCDone() override;
    void onRunAniWithSequenceDone() override;
    void onNextEffectNV();
    void onStartEffectNV();
    void pauseStarGuide();
    void resumStarGuide();
    std::vector<MSBookObject *> list_guide_effect;
    std::vector<MSBookObject *> list_ani_effect;
    int count_gaf_play = 0 , count_gaf_done = 0, count_gaf_sequence_done = 0, total_gaf_sequence = 0;
    
    void onTouchBegan();
    void onTouchMoved();
    void onTouchEnded();
    Vec2 touch_pos;
    void onNextPage();
    void onBackPage();
    void onRefresh();
    void stopAllActionAndEffect();
    void onKeyboardPage(int index_keyboard);
    
    MSStoryGuide * _guide;
    cocos2d::EventListenerTouchOneByOne * _touch_listener = nullptr;
    
    void displayEffectTextMC(std::string _str_object, float duration);
    bool touch_object_enable = false, isHighlightDone = false, is_start_star_effect = false;
    void fixSizeDisplay();
	void calculateStandardTime();
private:
	bool m_isGAFPlayedFirst = false;
	float m_totalTimeGAF = 0.0f;
};

CREATE_READER(MSBookPage);

#endif
