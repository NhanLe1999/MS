#ifndef GameMakeAMatch_hpp
#define GameMakeAMatch_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GameMakeAMatch_Item.h"

class MJGameMakeAMatch : public MJGameBase<MJGameMakeAMatch, mj::MakeAMatchDataSet>, public MJGameMakeAMatch_ItemDelegate {
public:
    static void initReader();
    CREATE_FUNC(MJGameMakeAMatch);
    static MJGameMakeAMatch* createGame(mj::GameDataSet<mj::MakeAMatchDataSet> data);
    static mj::GameDataSet<mj::MakeAMatchDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::MakeAMatchDataSet> generateData(rapidjson::Value value_data, std::string resource);
    static mj::GameDataSet<mj::MakeAMatchDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    void didLoadFromCSB(mj::GameDataSet<mj::MakeAMatchDataSet> data);
    void adjustDifficulity();
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;
    void onEnter() override;
    void onExitTransitionDidStart() override;
    void onRefresh(cocos2d::Ref* sender);
    void onClose(cocos2d::Ref* sender);

    mj::GameDataSet<mj::MakeAMatchDataSet> loadData();

    mj::MakeAMatchDataSet _current_set;

    void onFlipCardWithName(Ref* sender) override;

    std::vector<Vec2> list_pos;
    int _column = 0, _row = 0;
    void setColumnRowItem();
    int item_size_w = 210, item_size_h = 190;
    Layout* content_layout, * panel_object, * panel_results;
    ImageView* _results;
    ImageView* bg;
    int count_flip = 0, tag_flip_recent = 0, count_match_true = 0;
    void mergeTwoItems(MJGameMakeAMatch_Item* item1, MJGameMakeAMatch_Item* item2);
    void moveItemMergeToResults(MJGameMakeAMatch_Item* item);
    bool isRunAni = false;
    void onFinishGame();
    cocos2d::Size win_size;
    void runAnimStartGame();
    enum MakeAMatch_Type {
        TT_Type = 1,
        II_Type = 2,
        TI_Type = 3,
    };
    bool isTestGame = false;
    void radio();
    Button* _speaker;
    void guideGame();
    void stopGuideGame();
    float _audio = -1;
    cocos2d::Vec2 midle_pos;
    cocos2d::Vec2 results;
    cocos2d::Vec2 pan_results;
    float _boxResultSizeWidth = 0;
    int _arrObjectSize = 0;
};

CREATE_READER(MJGameMakeAMatch);

#endif /* GameMakeAMatch_hpp */