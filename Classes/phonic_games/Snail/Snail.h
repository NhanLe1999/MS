//
//  Snail.h
//

#ifndef Snail_h
#define Snail_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "Snail_Character.h"
#include "PhonicInfoModel.h"
#include "MSRecord.h"

using namespace cocos2d;
class Snail : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public Snail_CharacterDelegate, public MSRecordDelegate
{
public:
    CREATE_FUNC(Snail);
    static Snail *createGame(std::string json_file);
    
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void didLoadFromCSB(std::string json_file);
    void generateData(rapidjson::Value value_data);
    void onEnter() override;
    void onExitTransitionDidStar();
    void loadFrames();
    void showIntroGame();
	void onFinishGame() ;
    void setTouchCharacter_Item(bool enable = false);
    cocos2d::Vec2 pos_guide;
    void onSpeakerClicked(cocos2d::Ref *sender);
    cocos2d::ui::Button *speaker;
    std::string speaker_audio_path;
    

    struct SNAIL_INFO {
        std::string text;
        std::string audio_path_n;
        std::string audio_path_s;
        std::vector<Snail_Character::ITEM_INFO> list_character;
    };
    
    std::vector<SNAIL_INFO> list_item_info;
    SNAIL_INFO current_item;
    std::string text_current;
    int index_item = 0, index_character = 0;
    
    cocos2d::ui::ImageView *bridge;
    cocos2d::ui::Layout *root_layout, *character_layout;
    void createCloud();
    void moveCloud(cocos2d::ui::ImageView *cloud, float width);
    
    void createCharacter();
    std::vector<Snail_Character *> list_character;
    
    void runTextHighlight();
    void runTextHighlight(int idx);
    void runTextSlow();
    void runTextNormal();
    
    GAFWrapper *gaf_snail, *gaf_level1, *gaf_level2, *gaf_level3;
    void onCharacterPlayDone() override;
    cocos2d::ui::Layout *snail_layout, *touch_snail;
    void showSnail();
    float getPosYGafSnail(float pos_x);
    void onSnailClicked(cocos2d::Ref *sender);
    void onSnailWalk();
    void onSnailSkate();
    void onSnailJet();
    void onSnailHide();
    int level_snail = 0, index_walk = 0;
    bool is_rotate_snail = false;
    bool _enable_runTextHideLight = false;
    bool _enableShowHandGuidingRecord = true;
    bool _enableShowHandGuidingPlay = true;
    bool _enableShowHandinAllLevel = true;
    enum SNAIL_LEVEL {
		LEVEL_NONE = 0,
        LEVEL_WALK = 1,
        LEVEL_SKATE = 2,
        LEVEL_JET = 3
    };

    enum SoundTypeGuiding {
        // for guiding
        BEFORE_RECORD,
        ON_RECORD,
        AFTER_RECORD,
        ON_CONTINUE
    };
    
	void setBtnSnailLevel(SNAIL_LEVEL level);
    void onLevelClicked(cocos2d::Ref *sender);
    void createButtonLevel();
    bool is_btlevel_clicked = false;
	bool is_record_guided = false;
    int audio_action = 0;
    
    void disableAllBtns();
    void enableAllBtns();
    
    cocos2d::ui::Layout *btstop, *btrecord, *btplay;
    cocos2d::ui::Button* _speaker = nullptr;
    GAFWrapper *gaf_record, *gaf_stop, *gaf_play;
    void createButtonRecord();
    
    void onRecord(cocos2d::Ref *sender);
    void onStop(cocos2d::Ref *sender);
    void onPlay(cocos2d::Ref *sender);
    void onNext(cocos2d::Ref *sender);
    enum RECORD_TYPE {
        RECORD_DONE = 4,
        AUDIO_DONE = 5,
    };
    void startRecord();
    void startRecordForPlastForm();
    void onFinishRecord(int finish_type,std::string url) override;
    void runLayoutRecord(cocos2d::ui::Layout *layout, bool isShow);
    MSRecord *_record = nullptr;
    bool _is_recording = false;
    cocos2d::ui::ImageView *btnext, *btprev;
    bool is_show_next = false;
    void resetButtonLevel();
    void onHideGuideRecord(cocos2d::Ref *sender);
    
    void acceptPermissionCallback();
    void denyPermissioCallback();
    void scheduleGuidingByHandAndSound();
    void playSoundGuiding();
    void showGuidingByHand(Vec2 pos);
    void hideGuiding();
    
    cocos2d::ui::Layout *btlevel1, *btlevel2, *btlevel3;
    int index_level = 0;
    int audio_bg_id = -1;
    
//    int count_touch = 0;
    int count_walk = 0;
    int count_skate = 0;
    int count_jet = 0;
    SoundTypeGuiding _sound_type_guiding = SoundTypeGuiding::BEFORE_RECORD;
    Layout* micLayout = NULL;
};
CREATE_READER(Snail);
#endif /* MJ_Snail_h */
