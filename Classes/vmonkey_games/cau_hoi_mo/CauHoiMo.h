#ifndef CauHoiMo_HPP
#define CauHoiMo_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"

//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;

class CauHoiMo : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol{
public:
	CREATE_FUNC(CauHoiMo);
	static CauHoiMo * createGame();
	//    static CauHoiMo * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

private:
	void didLoadFromCSB();
	//    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
	//    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

		//data
	struct Question {
		std::string text;
		std::string audio;
		std::string image_path;
		std::string bg_path;
	};
	std::vector <Question> data_game;
	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;
	void showQuestion();
	
	void showTapGuideGame(Vec2 pos1, Vec2 pos2, float delay_time, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void hideTapGuideGame();

	void inActiveMode(float delay_time = 5.0f);
	Layout* root_layout;
	Layout* btn_next;
	Layout* btn_sound;
	ImageView* image_character;
	void nextGame();
	int audio_id_question;
	void onFinishGameVMonkey() override;
	Vec2 pos_character;
	std::string font_name = "fonts/Montserrat-Medium.ttf";
	int font_sz = 70;
	void guideGame();

};

CREATE_READER(CauHoiMo);

#endif
