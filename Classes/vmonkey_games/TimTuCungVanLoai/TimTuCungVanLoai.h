#ifndef TimTuCungVanLoai_HPP
#define TimTuCungVanLoai_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "TimTuCungVanLoai_Item.h"
#include "Math_Func.h"

//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class TimTuCungVanLoai : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public TimTuCungVanLoai_ItemDelegate {
public:
	CREATE_FUNC(TimTuCungVanLoai);
	static TimTuCungVanLoai * createGame();
	//    static TimTuCungVanLoai * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

private:
	void didLoadFromCSB();
	//    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
	//    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

		//data
	
	struct Question {
		std::string text_question;
		std::string audio_question;
		std::string text_phonic;
		std::string audio_phonic;
		bool is_phonic;
        std::string dau_cau;
	};
	struct DataGame {
		Question ques;
		std::vector<TimTuCungVanLoai_Item::Answer>answers;
	};
	std::vector <DataGame> data_game;

	std::vector <TimTuCungVanLoai_Item*> answers_item;

	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;
	void showQuestion();
	virtual void onClickedBox(cocos2d::Ref * sender) override;
	void showTapGuideGame(Vec2 pos1, Vec2 pos2, float delay_time, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
	void hideTapGuideGame();

	void inActiveMode(float delay_time = 5.0f);
	Layout* root_layout;
	Layout* btn_sound;
	void nextGame();
	int audio_id_question;
	void onFinishGameVMonkey();
	int so_luot_choi;
	int current_question = 0;
	std::string font_name = "fonts/Montserrat-Medium.ttf";
	int font_sz = 70;
	float audio_id_sound = -1;
	float audio_guide_game = -1;
	void guideGame();
	void stopGuideGame();
	Text* text_question;
	Button* _speaker;
};

CREATE_READER(TimTuCungVanLoai);

#endif
