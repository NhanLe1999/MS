
#ifndef MJGameTyping_hpp
#define MJGameTyping_hpp

#include "GameBase.h"
#include "GameData.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GameTyping_ItemAnswer.h"
#include "GAFWrapper.h"
#include "PhonicConfig.h"
#include "MJBaseActivity.h"

class MJGameTyping : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameTyping_ItemAnswerDelegate  {
public:
    CREATE_FUNC(MJGameTyping);
	static MJGameTyping * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
    //bool game_play_enable = true;
    //void pauseGame();
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);
private:
	void didLoadFromCSB(std::vector<math::resource::Image*> images);
    void loadFrames();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    bool isRunAni = false;
    void onStartGame();
    void onFinishGame();
    void setInputConfigGame();
    void onSpeakerClicked(cocos2d::Ref* sender);
    
    struct TypingDataSet {
        std::vector<MJGameTyping_ItemAnswer::ItemAnswerInfo> list_objects;
        std::vector<MJGameTyping_ItemAnswer::ItemDistracting> list_distracting;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
        std::string hoc_am_hoc_van;
    };

    TypingDataSet game_dataset;
    int object_index = 0, count_readword = 0, count_answer = 0;
    GAFWrapper *gaf_monkey_typing = nullptr, *gaf_word_object = nullptr, *gaf_bird = nullptr;
    cocos2d::ui::Text *text_object ;
    
    enum TYPING_SEQUENC {
        IDLE = 1,
        TYPING = 2,
        TALK = 3,
    };
    
    std::string getGafSequenceName(int type_sequence);
    void playMonkeyTypingWithSequenceType(int type_sequence, bool isloop = false);
    void onFinishMokeyTypingSequenceType(int type_sequence);
    int typing_sequence_type = 0;
    cocos2d::ui::ImageView *background_img = nullptr;
    void onClickText(cocos2d::Ref *sender);
    void displayAnswer();
    void onClickAnswer(cocos2d::Ref *sender) override;
    void onDoneAnswer(cocos2d::Ref *sender) override;
    void displayObject();
    std::vector<Vec2> list_pos;
    void playObject();
    int max_read_word = 1, max_keyboard_word = 1;
    int max_turn = 0;
    int max_item_typing = 3;
    int count_tap_guide = 0;
    int number_keyboard_server = 0;
    int old_rand_pos = -1;
    cocos2d::ui::Button* _speaker;
    int _currentAudio;
	//void showTapGuideGame(Vec2 position, float delay_time = 6.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	//    void hideTapGuideGame();

	void inActiveMode();
};

CREATE_READER(MJGameTyping);

#endif
