/*
*
* MexicanCactus_VM.h
* Created by minhtzy on 3/10/18
*/

#ifndef MJ_GAME_MEXICAN_CACTUS_VM_H__
#define MJ_GAME_MEXICAN_CACTUS_VM_H__

#include "MJMainGBase.h"
#include "GAFWrapper.h"
#include "mexicancactus_item_vm.h"
#include <map>
USING_NS_CC;
using namespace ui;

class MexicanCactus_VM : public MJMainGBase,public MexicanCactus_VM_ItemDelegate {

public :
	static MexicanCactus_VM * createGame(std::string json_file);
	void onEnter() override;
	void onExitTransitionDidStart() override;
	void onFinishGame() ;
	void onMovedCactusBegan(cocos2d::Ref * sender) override;
	//void onCactusMoved(cocos2d::Ref * sender) override;
	void onMovedCactusEnded(cocos2d::Ref * sender) override;
protected:
	struct MEXICAN_CACTUS_CHARACTER_INFO {
		std::string text;
		std::string audio;
		std::vector<int > list_index;
		std::vector<int> list_available_index;
	};
	struct MEXICAN_CACTUS_INFO{
		std::string text;
		std::string normal_audio;
        std::string text_ko_dau;
        std::string audio_ko_dau;
		std::string slow_audio;
        std::vector<MEXICAN_CACTUS_CHARACTER_INFO> list_character_sync_text;
		std::vector<MEXICAN_CACTUS_CHARACTER_INFO> list_character;
        MEXICAN_CACTUS_CHARACTER_INFO dau_cau;
        bool co_dau = false;
	};

	std::vector<MEXICAN_CACTUS_INFO> list_item_info;
	std::vector<MexicanCactus_VM_Item *> list_cactus_item;

	MexicanCactus_VM();
	void didLoadFromCSB(std::string json_file);
	void generateData();
	void loadFrames();

	void showIntroGame();
	void showGamename();
	void startGame();

	void onScrambleCactus(MexicanCactus_VM_Item * s_cactus,MexicanCactus_VM_Item * d_cactus);
	void showCactus();
	void createCactus();
	void changeInfoCactus();

	void checkWinGame();
	void onMergeWords();
	void onMergeWord(int index = 0, std::function<void()> callback = nullptr);
	void playSyncText(float delay_times);
	void playSlowText();
	void playNormalText();
	void onNextWord();
	void onNextGame();

	cocos2d::ui::Text *text_lb = nullptr;
    cocos2d::ui::Text *text_dau = nullptr;
	cocos2d::ui::Text *text_hint = nullptr;
	GAFWrapper * gaf_confetti;
	int wrong_count;
	void showHint();
	inline void setColorForText(cocos2d::Color3B color_text);
	void enableTouch();
	void disableTouch(int bg_opacity = 0);

	void showGuideGame(float delay_time);
	void hideGuideGame();
    void inActiveMode();
    std::string question_vi;
private:

	//void initCactus();
	void initText();

	void genCharacterIndex(MEXICAN_CACTUS_INFO& info);
	std::string m_configpath;
	cocos2d::ui::Layout * root_layout;
	cocos2d::ui::Layout * touch_layout;

	std::vector<cocos2d::Vec2> cactus_locations;
	std::vector<Vec2> getFrameLocations(int number,cocos2d::Vec2 center ,cocos2d::Size size, float padding = 0);
	
	int item_index;
	MEXICAN_CACTUS_INFO current_item_info;

	std::map<MexicanCactus_VM_Item *, int> map_index_items;
	bool checkCorrectIndex();
    bool checkRomaji(char c);
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	cocos2d::ui::Button * speaker;
	void onClickedSpeaker(cocos2d::Ref * sender);

	cocos2d::ui::Button * btnSkip;
	void onClickedSkip(cocos2d::Ref * sender);
    int speak_id = -1;
};



#endif // !MJ_GAME_MEXICAN_CACTUS_H__
