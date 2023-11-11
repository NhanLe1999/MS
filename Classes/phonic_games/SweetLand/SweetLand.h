
#ifndef SweetLand_HPP
#define SweetLand_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "SweetLand_Item.h"
#include "PhonicInfoModel.h"

USING_NS_CC;
using namespace ui;

class SweetLand : public MJMainGBase, public SweetLand_ItemDelegate {
public:
	CREATE_FUNC(SweetLand);
	static SweetLand * createGame(std::string json_file, bool isAutoPlay = false);
	//static SweetLand * createGame(mj::GAME_PHONIC_INFO list_PHONIC_INFO, bool isAutoPlay = false, std::string game_type = GAME_TYPE_PHONIC);
	void generateData(rapidjson::Value value_data);

private:
	void didLoadFromCSB(std::string json_file, bool isAutoPlay);
	void didLoadFromCSB(mj::GAME_PHONIC_INFO list_PHONIC_INFO, std::string game_type, bool isAutoPlay);
	mj::GAME_PHONIC_INFO _list_PHONIC_INFO;

	
	void loadFrames();
	void onEnter() override;
	void onExitTransitionDidStart() override;
	bool look_sound_button;
	void playSound(cocos2d::Ref* sender);
	void onClickedDelegate(cocos2d::Ref *sender) override;
	
	std::vector<Vec2> getFrameLocations(int number, cocos2d::Vec2 center, cocos2d::Size size, float padding = 0);

	void onAutoPlay();
	void autoPlayRun(float dt);
	void onFinishGame() ;

	void showIntroGame();
	void showGamename();

	void inActiveMode();
	void addSweetToLand();

	void runNatTo(cocos2d::Vec2 pos, float speed, std::function<void()> call_back);
	void playNatEffect(bool correct);
	std::vector<cocos2d::Vec2> randomPositionsInRect(cocos2d::Size item_size, cocos2d::Rect box_rect, int number_item);
	std::vector<int> randomArrayIndexes(int number, int size);
	void onNextStage();

	void nextGame();
	void showGame();
	void showQuestion();
	void showTextEndQuestion();

	std::vector<bool> _char_filled;
	void playSyncText();
	void playSlowText();
	void playNormalText();
	inline void setColorForText(cocos2d::Color3B color_text);

	std::string type_game;
	int count_fail;
	void showHint();
	struct word
	{
		std::string text;
		int result;
		std::string audio_path;
		std::string audio_slow_path;
		std::vector<std::string> phonic_audios;
		std::vector<std::string> phonics;
	};

	word current_data;
	std::vector<word>data_game;
	Layout* root_layout;
	std::vector<SweetLand_Item*> items;
	GAFWrapper* gaf_Nat;
	cocos2d::ui::Text * text_lb;
	Layout* back_layout;
	int so_luot_choi;
	int current_index;
	int current_result;
	int count_false = 0;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	int font_sz = 150;
	bool is_syllable;


	// speaker 

	cocos2d::ui::Button * m_speaker;
	//autoPlay
	int testing_step = 1;
	bool autoPlay = false;
	float _gafPlace = 0;
	bool _flag = true;
	Sprite* _backgoundGame = nullptr;

	void onClickedSpeaker(cocos2d::Ref* sender);
	void gafRun(cocos2d::Vec2 pos);
	void gafLeap();
	void gafLeapOrRun();
	void stopGafLeapOrRun();
	void stopAudioGuideGameAndSpeaker();
};


#endif
