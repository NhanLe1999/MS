#ifndef CompareAndContrast_HPP
#define CompareAndContrast_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "CompareAndContrast_Item.h"
//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;

class CompareAndContrast : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public CompareAndContrast_ItemDelegate {
public:
	CREATE_FUNC(CompareAndContrast);
	static CompareAndContrast * createGame();
//    static CompareAndContrast * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

private:
	void didLoadFromCSB();
//    void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
//    mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

	//data
	struct Character {
		std::string text;
		std::string audio;
		std::string image_path;
		std::string compare_1;
		std::string compare_2;
		std::string compare_3;
		std::string audio_1;
		std::string audio_2;
		std::string audio_3;
	};
	struct PairCharacter {
		std::vector<Character> characters;
	};
	std::vector<PairCharacter> data_game;
	std::vector<CompareAndContrast_Item*> items;
	
	struct BoardCharacter {
		ui::Text* t_name;
		ui::Text* t_compare_1;
		ui::Text* t_compare_2;
		ui::Text* t_compare_3;
		ui::ImageView* image;
		ui::ImageView* bg1;
		ui::Layout* bg2;
		ui::Layout* panel;
		ui::Layout* image_panel;
		int current_number;
		Character info;
		std::vector <CompareAndContrast_Item*> items;
	};
	GAFWrapper* gaf_effect_correct;
	std::vector<BoardCharacter> boards;

	std::vector<ImageView*> images;

	struct Answer {
		std::string text;
		std::string audio;
		int key;
	};
	std::vector<std::vector<Answer>> data_answers;
	CompareAndContrast_Item* currentItem;

	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;

	virtual void onClickedBox(cocos2d::Ref * sender) override;
	virtual void onMoveBeginBox(cocos2d::Ref * sender) override;
	virtual void onMoveEndedBox(cocos2d::Ref * sender) override;
	virtual void onMovingBox(cocos2d::Ref * sender) override;
	void onTapBoardImage(BoardCharacter board);
	void onAutoPlay();
	void autoPlayRun(float dt);
	void onFinishGame() override;

	void showIntroGame();
	void scheduleGuideByHand(Vec2 pos1, Vec2 pos2, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");

	void inActiveMode(float delay_time = 5.0f);
	void onNextStage();
	void nextGame();
	void nextStage();
	void showGame();
	void showQuestion();
	void showTextLBL(Text* text, float time, std::string path_sound);
	void trueAction(int index_board, CompareAndContrast_Item* item);
	void genBoards();
	void showBoards();
	void playGuidingBySound();
	std::string type_game;

	Layout* root_layout;
	
	bool kt_sound_interact;
	int key_interact;
	std::string gaf_guild_type;
	GAFWrapper* gaf_title;
	GAFWrapper* gaf_guild;
	GAFWrapper* gaf_charbg_1;
	GAFWrapper* gaf_charbg_2;

	std::vector <GAFWrapper*> score_stars;
	ImageView* image_scorebar;

	int so_luot_choi;
	int current_index; // de dem stage
	int current_answer_index; // de dem so dap an da dung 1 stage
	int old_zorder;
	int audio_id;
	int _answer_id = 0;
	bool _needGuidingByHand = true;
	bool _isTapBoardImage = false;


	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 70;
    bool tap = false;
    Vec2 start_pos, clone_pos;
   
    //hide img
    BoardCharacter showed_board;
    Vec2 pos_1, pos_2;
    void onHideImage();
    
	//autoPlay
	int testing_step = 1;

	bool autoPlay = false;

	// touch
	cocos2d::ui::Layout  * touch_layout;
	cocos2d::ui::Layout * back_layout;
	cocos2d::ui::Button* _speaker = nullptr;
	cocos2d::ui::Layout* _guide_layout = nullptr;
	void enableTouch();
	void disableTouch();
    
};

CREATE_READER(CompareAndContrast);

#endif
