
#ifndef GameQA_hpp
#define GameQA_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GameQA_Item.h"
#include "GameQA_Answer.h"

class MJGameQA : public MJGameBase<MJGameQA, mj::QADataSet>, public MJGameQA_ItemDelegate, public MJGameQA_AnswerDelegate {
public:
    static void initReader();
    CREATE_FUNC(MJGameQA);
    static MJGameQA* createGame(mj::GameDataSet<mj::QADataSet> data);
    static mj::GameDataSet<mj::QADataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::QADataSet> generateData(rapidjson::Value value_data, std::string resource);
    static mj::GameDataSet<mj::QADataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    void didLoadFromCSB(mj::GameDataSet<mj::QADataSet> data);
    void adjustDifficulity();
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
    void onEnter() override;
    void onExitTransitionDidStart() override;

    void onRefresh(cocos2d::Ref* sender);
    void onClose(cocos2d::Ref* sender);
    void checkAllQAItem(int tag, bool nextQues = true);
    void checkCountclickedQues();
    void setColorforAll(std::string color, int tag);

    mj::QADataSet _current_set;
    mj::GameDataSet<mj::QADataSet> loadData();
    void onChooseQuesItem(Ref* sender) override;
    void onChooseAnswer(Ref* sender) override;

    std::vector<Vec2> list_pos;
    bool isRunAni = false;
    bool _enableTouchItem = false;
    bool isEnableTouchItem();
    void onFinishGame();
    void scheduleSoundGuidingGame(int tag = -1);

    static bool is_auto_submit;
    int _numques = 0, itemcurrent_index = 0;
    void setInputConfigGame();

    Button* btdone = nullptr, * btnext = nullptr;
    ImageView* bg = nullptr, * img_btdone = nullptr, * alert_true = nullptr, * alert_false = nullptr;
    ImageView* _icon_white = nullptr;
    Layout* _layout_touch = nullptr;
    std::vector<MJGameQA_Item*> list_item;
    std::vector<MJGameQA_Answer*> list_answercontent;
    void onDoneGame(Ref* senfer);
    void onNext(Ref* senfer);
    Text* results_lb = nullptr, * title = nullptr;
    void onShowUI();
    cocos2d::Size win_size;
    void showArrowGuideDone();
    void hideArrowGuideDone();
    bool submit_arrowguide_showed = false;
    bool isVM;
    std::string audio_answer;
    bool isEndGame = false;
    bool _enableTouch = false;
    bool _allQuesWasChoosed = false;
};

CREATE_READER(MJGameQA);

#endif /* GameQA_hpp */
