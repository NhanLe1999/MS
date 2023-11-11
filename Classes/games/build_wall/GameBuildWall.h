
#ifndef MJGameBuildWall_hpp
#define MJGameBuildWall_hpp

#include "GameBase.h"
#include "GameData.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GameBuildWall_Item.h"
#include "GAFWrapper.h"
#include "GameBuildWall_Crocodile.h"
#define max_item_wall 4
#define wall_left 150.0f
#define wall_right 850.0f

#include "MJBaseActivity.h"


class MJGameBuildWall : public ms::BaseActivity, public cocostudio::WidgetCallBackHandlerProtocol, public MJGameBuildWall_ItemDelegate, public MJGameBuildWall_CrocodileDelegate  {
public:
    CREATE_FUNC(MJGameBuildWall);
	static MJGameBuildWall * createGame(std::vector<math::resource::Image*> images);
    void generateData(rapidjson::Value value_data);
	void readConfig(rapidjson::Value value_data, std::vector<math::resource::Image*> images);

    //bool game_play_enable = true;
    //void pauseGame();
private:
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
        std::vector<MJGameBuildWall_Item::ItemInfo> list_objects;
        std::string audio_title;
        std::string audio_intro;
        int game_id;
        std::string title;
        std::string intro;
    };
    
    GameDataSet game_dataset;
    int object_index = 0;
	int item_wall;
    MJGameBuildWall_Crocodile *crocodile = nullptr;
    cocos2d::ui::Layout *layout_content = nullptr;
    std::vector<MJGameBuildWall_Item *> list_item;
    void onCrocodileActionFinish(int action_type) override;
    int count_crocodile_jump = 0;
    void onChooseAnswerFinish() override;
    void onChooseWrongAnswer() override;
    GAFWrapper *explosive = nullptr;
    int is_answer_done = 0;
    void nextTurnPlay();
    int oderZ_item = 10, audio_bg_id = 0;
    void showItemQuestion();
    void onDoneGame();
    
    void showGuide(cocos2d::Point, cocos2d::Point);
    bool _is_guide_showed;
    bool _isPlayAudio = false;
    bool play_image;
    Button* guideBtn;
    std::string question_vi;
};

CREATE_READER(MJGameBuildWall);

#endif
