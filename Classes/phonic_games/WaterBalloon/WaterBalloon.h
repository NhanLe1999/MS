
#ifndef WaterBalloon_HPP
#define WaterBalloon_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "WaterBalloon_Item.h"
#include "PhonicInfoModel.h"
#include "StoryConfigManager.h"
USING_NS_CC;
using namespace ui;

class WaterBalloon : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public WaterBalloon_ItemDelegate {
public:
	CREATE_FUNC(WaterBalloon);
	static WaterBalloon * createGame(std::string json_file, bool isAutoPlay = false);
	static WaterBalloon * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data,std::vector<math::resource::Image*> images);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

private:
	void didLoadFromCSB(std::string json_file, bool isAutoPlay);
	void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
	mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

	//data
	struct Three_Words {
		std::vector<WaterBalloon_Item::Word> words;
	};
	std::vector<Three_Words> data_game;
	std::vector<WaterBalloon_Item*> items;
	std::vector<std::string> results;

	Layout* sound_button;

	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;
	void playSound(cocos2d::Ref* sender);
	void onClickedDelegate(cocos2d::Ref *sender, int id) override;
	
	void onAutoPlay();
	void autoPlayRun(float dt);
	void onFinishGame() ;

	void inActiveMode(float delay_time = 5.0f);
	void onNextStage();

	void nextGame();
	void showGame();
	void showQuestion();
	void nemBong(int id, bool is_true);
    int getItemFontSize();

	void showInformationGame();
    
	std::string type_game;

	GAFWrapper* gaf_bach_tuoc_1;
	GAFWrapper* gaf_bach_tuoc_2;
	GAFWrapper* gaf_bubbles_left_1;
	GAFWrapper* gaf_bubbles_left_2;
	GAFWrapper* gaf_bubbles_mid;
	GAFWrapper* gaf_bubbles_right_1;
	GAFWrapper* gaf_bubbles_right_2;
	GAFWrapper* gaf_bubbles_right_3;
	GAFWrapper* gaf_character;
	GAFWrapper* gaf_balloonB;
	GAFWrapper* gaf_balloonF;
	GAFWrapper* gaf_correct;
	int audio_id_tu;
	int audio_id_danh_van;

	Layout* root_layout;

	int so_luot_choi;
    std::string hoc_am_hoc_van = "";
	int current_index;
	int current_result;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 70;

	//autoPlay
	int testing_step = 1;

	bool autoPlay = false;
	bool flag = false;
};

CREATE_READER(WaterBalloon);

#endif
