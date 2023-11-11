#ifndef DanhGia_HPP
#define DanhGia_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "Math_Func.h"

//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class DanhGia : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol{
public:
	CREATE_FUNC(DanhGia);
	static DanhGia * createGame();
	//    static DanhGia * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
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
		std::string text_smile;
		std::string audio_smile;
		std::string path_smile;
		std::string text_sad;
		std::string audio_sad;
		std::string path_sad;
        std::string intro_question = "";
	};
	struct ImageQuestion {
		std::string audio;
		std::string image_path;
		bool is_smile;
	};
	struct DataGame {
		Question ques;
		std::vector<ImageQuestion>images_question;
        
	};
	std::vector <DataGame> data_game;



	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;
	void showQuestion();
    void startGame();
	void showTapGuideGame(Vec2 pos1, Vec2 pos2, float delay_time, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void hideTapGuideGame();

	void inActiveMode(float delay_time = 5.0f);
	Layout* root_layout;
	Button* btn_sound;
	Layout* btn_radio;
	Layout* star_smile_csb;
	Layout* star_sad_csb;

	ImageView* btn_true_red;
	ImageView* btn_true_blue;
	ImageView* btn_false_red;
	ImageView* btn_false_blue;
	ImageView* btn_select_true;
	ImageView* btn_select_false;
	Label * textTrue;
	Label * textFalse;
	//ImageView* image_question;
	Layout* panel_image;
	ImageView* model;
	void nextGame();
	int audio_id_question;
	void onFinishGame();
	int so_luot_choi;
    bool is_speak_answer = false;
	int current_question = 0;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 70;
	void enableTouch();
	void disableTouch();
	void gameTutorial();
	void pictureClick();
	void buttonTrueClick();
	void buttonFalseClick();
	void radio();

};

CREATE_READER(DanhGia);

#endif /* DanhGia_HPP */
