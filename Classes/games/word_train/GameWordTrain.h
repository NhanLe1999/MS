
#ifndef MJGameWordTrain_hpp
#define MJGameWordTrain_hpp

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GameWordTrain_Item.h"
#include "GAFWrapper.h"
#include "MJBaseActivity.h"

class MJGameWordTrain : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameWordTrain_ItemDelegate  {
public:
    CREATE_FUNC(MJGameWordTrain);
	static MJGameWordTrain * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
    //bool game_play_enable = true;
    //void pauseGame();
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
    std::string question_vi;
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    
    void onStartGame();
    void onFinishGame();
    void setInputConfigGame();
    
    struct GameDataSet {
        std::vector<MJGameWordTrain_Item::ItemInfo> list_objects;
        std::vector<MJGameWordTrain_Item::ItemDistracting> list_distracting;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
    };
    GameDataSet game_dataset;
    int object_index = 0;
    
    GAFWrapper *train = nullptr;
    GAFWrapper *_bling = nullptr;
    cocos2d::ui::Layout * _panelBoard = nullptr;
    cocos2d::ui::ImageView *board = nullptr;
    Vec2 pos_train;
    void onFinishTurn() override;
    void onAnswerTrue() override;
    void onTouchItemBegan() override;
    void onTouchItemEnded() override;
    void stopAudioIndex(int index);
    void stopListAudio();

    bool m_speakable;
    void showGuide();
    void inActiveMode();
    void unscheduleInActiveMode();
    bool _handShowed = false;
    int _speakerAudioPath = -1;
    int _imageAudioPath = -1;
    int _inactiveAudioPath = -1;
    cocos2d::ui::Button* _speaker;
};

CREATE_READER(MJGameWordTrain);

#endif
