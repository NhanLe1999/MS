#ifndef LuaChon_HPP
#define LuaChon_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "LuaChon_Item.h"
#include "Math_Func.h"

//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class LuaChon : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public LuaChon_ItemDelegate {
public:
	CREATE_FUNC(LuaChon);
	static LuaChon* createGame();
	//    static LuaChon * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string& callback_name) override;

private:
	void didLoadFromCSB();
	//    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
	//    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

		//data
	struct Question {
		std::string text;
		std::string audio;
		std::string image_path;
		bool visible_image;
	};
	struct Answer {
		std::string audio;
		std::string image_path;
		bool is_true;
	};
	struct DataGame {
		Question ques;
		std::vector<Answer>answers;
	};
	std::vector <DataGame> data_game;

	std::vector <LuaChon_Item*> answers_item;
	std::string path_background;
	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;
	void showQuestion();
	virtual void onClickedBox(cocos2d::Ref* sender) override;
	void showTapGuideGame2(Vec2 pos, int loop = 100, float delay_time = 8.0f, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void showTutorial();
	void stopGuiding();
	void inActiveMode(float delay_time = 5.0f);
	Layout* root_layout;
	Layout* btn_sound;
	ImageView* image_character;
	void nextGame();
	int audio_id_question = -1;
	void onFinishGameVMonkey();
	int so_luot_choi;
	int current_question = 0;
	std::string font_name = "fonts/Montserrat-Medium.ttf";
	int font_sz = 70;
	Button* _speaker;
	int _audio = -1;
};

CREATE_READER(LuaChon);

#endif