
#ifndef GameLandThePlane_hpp
#define GameLandThePlane_hpp

#include "cocostudiomacro2.h"
#include "GameBase.h"
#include "GameData.h"
#include "GameLandThePlane_Item.h"
#include "GAFWrapper.h"
#include "StoryConfigManager.h"
class MJGameLandThePlane : public MJGameBase<MJGameLandThePlane, mj::LandThePlaneDataSet>, public MJGameLandThePlane_ItemDelegate {
public:
    static void initReader();
    CREATE_FUNC(MJGameLandThePlane);
    static MJGameLandThePlane * createGame(mj::GameDataSet<mj::LandThePlaneDataSet> data);
    static mj::GameDataSet<mj::LandThePlaneDataSet> generateData(cocos2d::ValueMap value_map, std::string resource);
    static mj::GameDataSet<mj::LandThePlaneDataSet> generateData(rapidjson::Value value_data, std::string resource);
	static mj::GameDataSet<mj::LandThePlaneDataSet> generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    void didLoadFromCSB(mj::GameDataSet<mj::LandThePlaneDataSet> data);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    void onExitTransitionDidStart() override;
    void onRefresh(cocos2d::Ref * sender);
    void onClose(cocos2d::Ref * sender);
    cocos2d::Size win_size;
    
    mj::LandThePlaneDataSet _current_set;
    mj::GameDataSet<mj::LandThePlaneDataSet> loadData();
    void onClickLand(Ref *sender) override;
    
    bool isRunAni = false;
    void onFinishGame();
    
    int _gamelevel, _numkeywords, _numturnplay, _count_turnplay_done, _count_keywords_done;
    void setInputConfigGame();
    Layout *_content_layout = nullptr;
    GAFWrapper *_objectplayer = nullptr;
    void addObjectPlayer();
    void loadItemTurnPlay();
    std::vector<Vec2> list_pos;
    int tag_item = 100, tag_item_remove = 100;
    std::vector<int> list_id_items;
    std::string word_answer;
    std::vector<MJGameLandThePlane_Item *> list_items;
    void moveObjectPlayer(Vec2 pos_to);
    float getAngelObjectPlayer(Vec2 pos_objectplayer, Vec2 pos_to);
    void removeItemLand();
    void checkNextKeyWord();
    bool isDoneGame = false;
    void loadCloud();
    void runAnimStartGame();
    void loadGuideAnswer();
    void inActiveMode();
    void showHand();
    void hideHand();
    mj::LandThePlaneDataSet item_data;
    int au_id_plane = -1, au_id_startplane = -1;
    int guide_answer = false;
    int font_sz = 42;

    cocos2d::ui::Button* _speaker;
    bool _isSpeaking = false;
    void onSpeakerClicked(cocos2d::Ref* sender);
    int _audioId;
    bool _guideShowed;
    int _audioFly;
};

CREATE_READER(MJGameLandThePlane);

#endif /* GameLandThePlane_hpp */
