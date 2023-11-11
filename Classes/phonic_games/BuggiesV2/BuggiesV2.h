//
//  BuggiesV2.h
//

#ifndef BuggiesV2_h
#define BuggiesV2_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "BuggiesV2_Item.h"
#include "PhonicInfoModel.h"
#include "BuggiesV2_ChooseCar.h"
using namespace cocos2d;
class BuggiesV2 : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public BuggiesV2_ChooseCarDelegate
{
public:
    CREATE_FUNC(BuggiesV2);
    static BuggiesV2 *createGame(std::string json_file);
    std::string _scheduleReminderName = "RemindStudy";
    
private:
    void didLoadFromCSB(std::string json_file);
    void generateData(rapidjson::Value value_data);
	
    void onEnter() override;
    void onExitTransitionDidStart() override;
    void loadFrames();
    void showIntroGame();
    cocos2d::Vec2 pos_guide;
    void onSpeakerClicked(cocos2d::Ref *sender);
    cocos2d::ui::Button *speaker;
    std::string speaker_audio_path;
    
    struct CHARACTER_INFO {
        std::string character;
        std::string audio_path;
        std::vector<int> list_index;
    };
    
    struct BUGGIES_INFO {
		std::string level;
        std::string text;
        std::string audio_path_slow;
        std::string audio_path_normal;
        std::vector<CHARACTER_INFO> list_character;
    };
	struct Camera {
		cocos2d::Vec2 position;
		float scale;

		Camera() :position{ cocos2d::Vec2(0,0) }, scale{ 1 } {}
	};

	Camera m_camera;
	void genCharacterIndex(BUGGIES_INFO &info);
    std::vector<BUGGIES_INFO> list_item_info;
    BUGGIES_INFO current_item_info;
    CHARACTER_INFO current_character_info;
    int index_item = 0, index_character = 0;
    
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;

	ui::Text * text_hint;
	void showHint(float delay_time = 15.0f,bool fade_out = false);
	void hideHint();

	void runText();
	Sequence * createMoveAnimation(std::vector<Vec2> st, float speed = 255);
	std::vector<Vec2> createCirclePath(Vec2 center_pos, float radius, int segment = 16, float start_angle = 0, float arc_angle = 360, bool backToStart = true);
	std::vector<Vec2> createZigzagPath(Vec2 center_pos, float radius1, float radius2, int segment = 16, float start_angle = 0, float arc_angle = 360, bool backToStart = true);
	void moveTextByCirclePath();
	void moveTextZigzagPath();

    Vec2 touch_point;
    cocos2d::ui::ImageView *car;
    bool is_touch_car = false;
    void update(float dt) override;
    void updateCar(float dt);
	void updateCamera();
    cocos2d::MotionStreak *motion_streak;
    void showText();
    void showCharacter();
    cocos2d::ui::Layout *gachngang_layout, *touch_layout, *m_worldNode, *m_bg_layout;
    std::vector<cocos2d::ui::ImageView *> list_gachngang;
    void runEffectGachNgang();
    cocos2d::ui::Text *text_lb = nullptr;
    std::vector<Vec2> list_pos_letter;
    std::vector<cocos2d::Sprite *> list_letter;
    void setColorForText(Color3B color_text);
    std::vector<BuggiesV2_Item *> list_character;
    void mergeLetter(BuggiesV2_Item *character_item);
    void onNextCharacter();
    void onNextWord();
    Vec2 getPositionCharacterMove();

	void playSyncText();
    void playSlowText();
    void playNormalText();
    void mergeWord();
    void disableTouch();
    void enableTouch();
    void resetCar();
	void stopCarEffect();
    void showChooseCarView();
    void onChooseCar(std::string car_index, int map_index) override;
    void scheduleReminder();
    void stopChainSpelling();

    int map_index;
    std::string car_index;
    cocos2d::ui::ImageView* _imgTextBg = nullptr;
    Vec2 _minUnderPos;
    std::vector<std::string> _chainSpell;
    bool _isIntroSoundStop = false;

    
};
CREATE_READER(BuggiesV2);
#endif /* MJ_BuggiesV2_h */
