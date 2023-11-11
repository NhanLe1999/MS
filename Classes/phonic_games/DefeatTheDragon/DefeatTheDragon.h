
#ifndef DefeatTheDragon_HPP
#define DefeatTheDragon_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "DefeatTheDragon_Item.h"
#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;

class DefeatTheDragon : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public DefeatTheDragon_ItemDelegate {
public:
	CREATE_FUNC(DefeatTheDragon);
	static DefeatTheDragon * createGame(std::string json_file, bool isAutoPlay = false);
	static DefeatTheDragon * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
private:
	void didLoadFromCSB(std::string json_file, bool isAutoPlay);
	void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
	bool look_sound_button;
	mj::GAME_PHONIC_INFO _list_PHONIC_INFO;
	//    std::vector<mj::PHONIC_INFO> _list_phonic_info;
	//    std::vector<mj::PHONIC_INFO> _sublist_phonic_info;

	//    mj::PHONIC_INFO phonic_info;
	struct Phonic {
		std::string text;
		std::string audio;
        bool is_right = false;
	};
    
	struct Word {
		std::string full_text;
		std::string audio_full_text;
		std::vector<Phonic> phonics;
	};
	std::vector<Word> data;

	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	//    void onRefresh();
	void onClickedDelegate(cocos2d::Ref *sender, ui::Text* text, int id) override;

	
	void showHint(float dt = 10.0f);
	void hideHint();

	//void update(float dt = 20.0f) override;
	void checkIdle();
	void onAutoPlay();
	void autoPlayRun(float dt);
	void onFinishGame() ;
	void playSound(cocos2d::Ref* sender);

	void onNextStage();

	void nextGame();
	void showGame();


	void dragonShowText(int index);
	void attackDragonNormal();
	void attackDragonUltimate();
	void dragonFly();
	void dragonBreathe();
	void dragonFallAndGU();
	std::vector <DefeatTheDragon_Item*> items;
	std::vector <Vec2> pos1;
	std::vector <Vec2> pos2;
	//void hide_full_text();
	std::string type_game;

	Text* game_name;
	GAFWrapper* background_gaf;
	GAFWrapper* knight_gaf;
	GAFWrapper* knight_effect_gaf;
	GAFWrapper* dragon_gaf;
	bool check_dragon_idle = false;
	GAFWrapper* dragon_effect_gaf;
	GAFWrapper* fire_large_gaf;
	GAFWrapper* bg_win_gaf;
	GAFWrapper* fire_small_gaf;
	GAFWrapper* _background_gaf;

	Layout* x_layout;
	Layout* root_layout;
	Layout* item_layout;

	int number_words;
	int current_index;

	Text* text_full;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 72;

	//autoPlay
	int testing_step = 1;

	bool autoPlay = false;

	int id_audio_question;
    
    int _numberShowHint = 0;
    
    std::string _pathAudioCorrect = "";

	Sprite* _backgoundGame;
};

CREATE_READER(DefeatTheDragon);

#endif
