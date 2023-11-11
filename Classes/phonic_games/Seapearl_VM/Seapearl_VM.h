//
// Seapearl_VM.h
// Created by Minhtzy on 19/11/2018
//

#ifndef Seapearl_VM_h
#define Seapearl_VM_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "Seapearl_VM_Item.h"
#include "PhonicInfoModel.h"

using namespace cocos2d;
class Seapearl_VM : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol,Seapearl_VM_ItemDelegate
{
public:
	CREATE_FUNC(Seapearl_VM);
	static Seapearl_VM *createGame(std::string json_file);

private:

	void didLoadFromCSB(std::string json_file);
	std::string m_configPath;
	bool is_show_image;
	void generateData();

	void onEnter() override;
	void onExitTransitionDidStar();
	void loadFrames();
	void showIntroGame();
	void startGame();
	cocos2d::Vec2 pos_guide;
	void onSpeakerClicked(cocos2d::Ref *sender);
	cocos2d::ui::Button *speaker;
	std::string speaker_audio_path;

	struct CHARACTER_INFO {
		std::string character;
		std::string audio_path;
		std::vector<int> list_index;
	};

	struct Seapearl_VM_INFO {
		std::string text;
		std::string audio_path_slow;
		std::string audio_path_normal;
        std::string text_khong_dau;
        std::string audio_khong_dau;
        std::string text_dau;
        std::string audio_dau;
        int pos_dau;
        bool co_dau;
		std::vector<CHARACTER_INFO> list_character;
		std::vector<CHARACTER_INFO> list_distracting;
		std::string image_path;
	};

	void runFish(Seapearl_VM_Item * act_fish, Seapearl_VM_Item * target_fish,std::function<void()> call_back = nullptr);

	void genCharacterIndex(Seapearl_VM_INFO &info);
	std::vector<Seapearl_VM_INFO> list_item_info;
	Seapearl_VM_INFO current_item_info;
	CHARACTER_INFO current_character_info;
	int index_item = 0, index_character = 0;

	void playClam(int state, std::function<void(gaf::GAFObject * obj, const std::string & callback_name)> callback = nullptr);
	void playClam();
    bool state_dau = false;
	int clam_state = 1; // 1. close 2. sight 3. open
	GAFWrapper * gaf_clam, *gaf_bgwin;
	ui::ImageView * img_bgwin;
	ui::ImageView * img_character;
    ui::ImageView * fake_gaf_bgwin;
    ui::ImageView * fake_gaf_bgwin_2;
	std::string font_name = "fonts/danhvan.ttf";

	Vec2 getPositionCharacterMove();

	void update(float dt) override;

	void onItemClicked(cocos2d::Ref * sender) override;
	void chooseRight(cocos2d::Ref * sender);
	void chooseWrong(cocos2d::Ref * sender);
	void showText();
	void showWord();
	void showCharacter();
	cocos2d::ui::Layout *gachngang_layout, *touch_layout, *m_bg_layout, * m_root_layout;
	std::vector<cocos2d::ui::ImageView *> list_gachngang;
	void runEffectGachNgang();
	cocos2d::ui::Text *text_lb = nullptr;
	std::vector<Vec2> list_pos_letter;
	std::vector<cocos2d::Sprite*> list_letter;
	void setColorForText(Color3B color_text);
	std::vector<Seapearl_VM_Item *> list_character;
	void mergeLetter(Seapearl_VM_Item *character_item);
	void onNextCharacter();
	void onNextWord();
	std::vector<Seapearl_VM_Item::CHARACTER_INFO> getDistracting(std::string key);


	void playSyncText();
	void playSlowText();
	void playNormalText();
	void mergeWord();
	void disableTouch();
	void enableTouch();
    bool checkRomaji(char c);
    std::string question_vi;
    cocos2d::ui::Text* text_dau;
};
CREATE_READER(Seapearl_VM);
#endif /* MJ_Seapearl_VM_h */
