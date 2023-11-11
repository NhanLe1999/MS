#define ChoBoPhanTimTongThe_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "ChoBoPhanTimTongThe_Item.h"
#include "Math_Func.h"

//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;
class ChoBoPhanTimTongThe : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public ChoBoPhanTimTongThe_ItemDelegate {
public:
	CREATE_FUNC(ChoBoPhanTimTongThe);
	static ChoBoPhanTimTongThe * createGame();
	//    static ChoBoPhanTimTongThe * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

private:
	void didLoadFromCSB();
	//    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
	//    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

		//data
	struct bo_phan {
		std::string audio;
		std::string image_path;
	};
	struct Question {
		std::string text;
		std::string audio;
		std::vector<bo_phan> images_question;
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

	std::vector <ChoBoPhanTimTongThe_Item*> answers_item;
	std::vector<ImageView*> imgs_ques;
	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool lock_sound_button;
	void showQuestion();
	virtual void onClickedBox(cocos2d::Ref * sender) override;

	void guideGame();
	void stopGuideGame();
	Layout* root_layout;
	Layout* btn_sound;
	Button* _speaker;
	void nextGame();
	int audio_id_question;
	void onFinishGameVMonkey() override;
	int so_luot_choi;
	int current_question = 0;
	std::string font_name = "fonts/Montserrat-Medium.ttf";
	int font_sz = 70;
	void imgFadeIn(int i, vector<ImageView*> imgs_ques);
	float _audio = -1;
	float _audio2 = -1;
	Sprite* _backgound = nullptr;
	Sprite* _backgoundGame = nullptr;
};

CREATE_READER(ChoBoPhanTimTongThe);
