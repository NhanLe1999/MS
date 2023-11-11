/*
*
* MexicanCactus.h
* Created by minhtzy on 3/10/18
*/

#ifndef MJ_GAME_MEXICAN_CACTUS_H__
#define MJ_GAME_MEXICAN_CACTUS_H__

#include "MJMainGBase.h"
#include "GAFWrapper.h"
#include "MexicanCactus_Item.h"
#include <map>
USING_NS_CC;
using namespace ui;

class MexicanCactus : public MJMainGBase,public MexicanCactus_ItemDelegate {

public :
	static MexicanCactus * createGame(std::string json_file);
	void onEnter() override;
	void onExitTransitionDidStart() override;
	void onFinishGame() ;
	void onMovedCactusBegan(cocos2d::Ref * sender) override;
	void onMovedCactusEnded(cocos2d::Ref * sender) override;
	void onExit() override;
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
		std::string slow_audio;
		std::vector<MEXICAN_CACTUS_CHARACTER_INFO> list_character;
	};

	std::vector<MEXICAN_CACTUS_INFO> list_item_info;
	std::vector<MexicanCactus_Item *> list_cactus_item;

	std::vector<MexicanCactus_Item*> _listCactusItemClone;

	MexicanCactus();
	void didLoadFromCSB(std::string json_file);
	void generateData();
	void loadFrames();

	void showIntroGame();
	void showGamename();
	void startGame();

	void onScrambleCactus(MexicanCactus_Item * s_cactus,MexicanCactus_Item * d_cactus);
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
	cocos2d::ui::Text *text_hint = nullptr;
	GAFWrapper * gaf_confetti;
	int wrong_count;
	void showHint();
	inline void setColorForText(cocos2d::Color3B color_text);
	void enableTouch();
	void disableTouch(int bg_opacity = 0);

    void inActiveMode();
	void stopInActiveMode();
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

	std::map<MexicanCactus_Item *, int> map_index_items;
	bool checkCorrectIndex();

	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";
	cocos2d::ui::Button * speaker;
	void onClickedSpeaker(cocos2d::Ref * sender);
	void skipAudioSpeaker();

	cocos2d::ui::Button * btnSkip;
	void onClickedSkip(cocos2d::Ref * sender);

	Sprite* _gameNameImage = nullptr;

	bool _flagGuideGame = true;
	bool _flag = true;
	Vec2 _endPos;
	Vec2 _startPos;
	bool _flagSpeaker = true;
	bool _flagInActiveMove = true;
};



#endif // !MJ_GAME_MEXICAN_CACTUS_H__
