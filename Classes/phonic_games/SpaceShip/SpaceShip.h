//
//  SpaceShip.h
//

#ifndef SpaceShip_h
#define SpaceShip_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "SpaceShip_Item.h"
#include "PhonicInfoModel.h"
#include "SpaceShip_Blur.h"
using namespace cocos2d;
class SpaceShip : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public SpaceShip_ItemDelegate
{
public:
    CREATE_FUNC(SpaceShip);
    static SpaceShip *createGame(std::string json_file,bool is_auto_play = false);
    
private:
    void didLoadFromCSB(std::string json_file, bool is_auto_play);
    void generateData(rapidjson::Value value_data);
    void onEnter() override;
    void onExitTransitionDidStar();
    void loadFrames();
    void showIntroGame();
    cocos2d::Vec2 pos_guide;
    void onSpeakerClicked(cocos2d::Ref *sender);
    cocos2d::ui::Button *speaker;
    std::string speaker_audio_path;
    
    struct CHARACTER_INFO {
        SpaceShip_Item::METEORITE_INFO meteorite_info;
        std::vector<SpaceShip_Item::METEORITE_INFO> list_distracting;
        std::vector<int> list_index;
    };
    
    struct SPACESHIP_INFO {
        std::string text;
        std::string audio_path_n;
        std::string audio_path_s;
		std::string image_path;
        std::vector<CHARACTER_INFO> list_character;
    };
    
    std::vector<SPACESHIP_INFO> list_item_info;
    SPACESHIP_INFO current_item;
    CHARACTER_INFO current_character;
    int index_item = 0, index_character= 0;
	bool is_show_image;

    SpaceShip_Blur *blur_planet, *blur_star, *blur_background;
    SpaceShip_Item *ship;
    std::vector<SpaceShip_Item *> list_meteorite;
    void disableTouch();
    void enableTouch();
    void onItemClicked(cocos2d::Ref *sender) override;
    cocos2d::ui::Layout *touch_layout;
    void chooseRight(SpaceShip_Item *item);
    void chooseWrong(SpaceShip_Item *item);
    void onNextCharacter();
    void onNextWord();
    
    void createMeteorite();
    
    cocos2d::ui::Layout *gachngang_layout;
    std::vector<cocos2d::ui::ImageView *> list_gachngang;
    cocos2d::ui::Text *text_lb = nullptr;
    cocos2d::ui::ImageView *img_character = nullptr;
    std::vector<Vec2> list_pos_letter;
    std::vector<cocos2d::Sprite *> list_letter;
    void runEffectGachNgang();
    void showText();
    void mergeCharacter(Vec2 pos_text);
    void mergeWord();
	void playSyncText();
    void playSlowText();
    void playNormalText();
    void setColorForText(Color3B color_text);
	std::vector<SpaceShip_Item::METEORITE_INFO> getDistracting(std::string key, std::vector<SpaceShip_Item::METEORITE_INFO> distracting);
	void genCharacterIndex(SPACESHIP_INFO &ship_info);
	void trimUnderline(SPACESHIP_INFO &ship_info);
    
    void inactiveMode();
    void stopInactiveMode();
	// Auto play
	bool is_auto_play;
	void autoPlay();
	int auto_play_index{ 0 };

    Sprite* _bgGameNameImage = nullptr;
    ImageView* _bgGame2 = nullptr;
};
CREATE_READER(SpaceShip);
#endif /* MJ_SpaceShip_h */
