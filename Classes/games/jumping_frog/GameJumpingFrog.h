
#ifndef GameJumpingFrog_hpp
#define GameJumpingFrog_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GameJumpingFrog_Item.h"
#include "GAFWrapper.h"

class MJGameJumpingFrog : public MJGameBase<MJGameJumpingFrog, mj::JumpingFrogDataSet>, public MJGameJumpingFrog_ItemDelegate {
public:
    static void initReader();
    CREATE_FUNC(MJGameJumpingFrog);
    static MJGameJumpingFrog * createGame(mj::GameDataSet<mj::JumpingFrogDataSet> data);
    static mj::GameDataSet<mj::JumpingFrogDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::JumpingFrogDataSet> generateData(rapidjson::Value value_data, std::string resource);
	static mj::GameDataSet<mj::JumpingFrogDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    void didLoadFromCSB(mj::GameDataSet<mj::JumpingFrogDataSet> data);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onRefresh(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);
    cocos2d::Size win_size;

    mj::JumpingFrogDataSet _current_set;
    mj::GameDataSet<mj::JumpingFrogDataSet> loadData();
    void onClickItem(Ref *sender) override;
    void onEnter() override;
    void onExitTransitionDidStart() override;
    
    bool isRunAni = false;
    void onFinishGame();
    
    int _gamelevel, _numkeywords, _numturnplay, _count_turnplay_done, _count_keywords_done;
    void setInputConfigGame();
    Layout *_content_layout = nullptr;
    GAFWrapper *_objectplayer = nullptr;
    void addObjectPlayer();
    void loadItemTurnPlay();
    std::vector<Vec2> list_pos;
    int tag_item = 100, tag_item_remove = 100, tag_item_clicked = 0, tag_item_recent = 0;
    std::vector<int> list_id_items;
    std::string word_answer;
    std::vector<MJGameJumpingFrog_Item *> list_items;
    void moveObjectPlayer(Vec2 pos_to);
    float getAngelObjectPlayer(Vec2 pos_objectplayer, Vec2 pos_to);
    void removeItemLand();
    void checkNextKeyWord();
    bool isDoneGame = false;
    void addObjectTrangTri();
    void runAnimStartGame();
    void loadGuideAnswer();
    mj::JumpingFrogDataSet item_data;
    int guide_answer = false;
    int font_sz = 80;
    void inActiveMode();
};

CREATE_READER(MJGameJumpingFrog);

#endif /* GameJumpingFrog_hpp */
