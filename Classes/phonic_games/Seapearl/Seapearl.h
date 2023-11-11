//
// Seapearl.h
// Created by Minhtzy on 19/11/2018
//

#ifndef Seapearl_h
#define Seapearl_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "Seapearl_Item.h"
#include "PhonicInfoModel.h"

using namespace cocos2d;
class Seapearl : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol,Seapearl_ItemDelegate
{
public:
	CREATE_FUNC(Seapearl);
	static Seapearl *createGame(std::string json_file);

private:

	void didLoadFromCSB(std::string json_file);
	std::string m_configPath;
	bool is_show_image;
	bool _readVowel;
	std::string _introAudio = "";
	bool hide_text;
	void generateData();

	void onEnter() override;
	void onExitTransitionDidStar();
	void loadFrames();
	void showIntroGame();
	void startGame();
	void playWinEffect();
	cocos2d::Vec2 pos_guide;
	void onSpeakerClicked(cocos2d::Ref *sender);
	cocos2d::ui::Button *speaker;
	std::string speaker_audio_path;
	GAFWrapper* _star1;
	GAFWrapper* _star2;
	GAFWrapper* _star3;
	GAFWrapper* _star4;
	struct CHARACTER_INFO {
		std::string character;
		std::string audio_path;
		std::vector<int> list_index;
	};

	struct SEAPEARL_INFO {
		std::string text;
		std::string audio_path_slow;
		std::string audio_path_normal;
		std::vector<CHARACTER_INFO> list_character;
		std::vector<CHARACTER_INFO> list_distracting;
		std::string image_path;
	};

	void runFish(Seapearl_Item * act_fish, Seapearl_Item * target_fish,std::function<void()> call_back = nullptr);

	void genCharacterIndex(SEAPEARL_INFO &info);
	std::vector<SEAPEARL_INFO> list_item_info;
	SEAPEARL_INFO current_item_info;
	CHARACTER_INFO current_character_info;
	int index_item = 0, index_character = 0;
	int _countVowel = 0;

	void playClam(int state, std::function<void(gaf::GAFObject * obj, const std::string & callback_name)> callback = nullptr);
	void playClam();

	bool _isSpeaking;
	int clam_state = 1; // 1. close 2. sight 3. open
	GAFWrapper * gaf_clam, *gaf_bgwin;
	ui::ImageView * img_bgwin;
	ui::ImageView * img_character;
	ui::ImageView * _imgBegin;
	ui::Text	  *	_txtBegin;
    ui::ImageView * fake_gaf_bgwin;
    ui::ImageView * fake_gaf_bgwin_2;
	std::string font_name = "fonts/KGWHATTHETEACHERWANTS.TTF";

	Vec2 getPositionCharacterMove();

	void update(float dt) override;

	void onItemClicked(cocos2d::Ref * sender) override;
	void chooseRight(cocos2d::Ref * sender);
	void chooseWrong(cocos2d::Ref * sender);
	void showText();
	void showWord();
	void ShowImageAndWordBeforeStart();
	void showCharacter();
	cocos2d::ui::Layout *gachngang_layout, *touch_layout, *m_bg_layout, * m_root_layout;
	std::vector<cocos2d::ui::ImageView *> list_gachngang;
	void runEffectGachNgang();
	cocos2d::ui::Text *text_lb = nullptr;
	std::vector<Vec2> list_pos_letter;
	std::vector<cocos2d::Sprite *> list_letter;
	void setColorForText(Color3B color_text);
	std::vector<Seapearl_Item *> list_character;
	void mergeLetter(Seapearl_Item *character_item);
	void onNextCharacter();
	void onNextWord();
	std::vector<Seapearl_Item::CHARACTER_INFO> getDistracting(std::string key);
	void inActiveMode();

	void playSyncText();
	void playSlowText();
	void playNormalText();
	void mergeWord();
	void disableTouch();
	void enableTouch();
};
CREATE_READER(Seapearl);
#endif /* MJ_Seapearl_h */
