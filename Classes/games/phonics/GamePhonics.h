
#ifndef MJGamePhonics_hpp
#define MJGamePhonics_hpp

#include "GameBase.h"
#include "GameData.h"
#include "cocostudiomacro2.h"
#include "GamePhonics_TextVideo.h"
#include "GamePhonics_Monkey.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJBaseActivity.h"

class MJGamePhonics : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGamePhonics_TextVideoDelegate, public MJGamePhonics_MonkeyDelegate {
public:
    CREATE_FUNC(MJGamePhonics);
	static MJGamePhonics * createGame(std::vector<math::resource::Image*> images);
	void generateData(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
    //bool game_play_enable = true;
    //void pauseGame();
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    std::vector<Vec2> list_pos;
    bool isRunAni = false;
    void onStartGame();
    void onFinishGame();
    void setInputConfigGame();
    void onFinishAction(int finish_type) override;
    void onFinishMonkey(int finish_type) override;
    void onRecordClicked() override;
    void onPlayClicked() override;
    MJGamePhonics_Monkey *monkey_mc = nullptr;
    MJGamePhonics_TextVideo *text_video = nullptr;
    
    struct PhonicsDataSet {
        std::vector<MJGamePhonics_TextVideo::ObjectInfo> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
    };
    PhonicsDataSet game_dataset;
    int object_index = 0;
    
    void onCloseGame() override;
    int audio_record_id = -1;
    int count_tap_guide = 0;
    void onNextTurnClicked() override;
    
};

CREATE_READER(MJGamePhonics);

#endif 
