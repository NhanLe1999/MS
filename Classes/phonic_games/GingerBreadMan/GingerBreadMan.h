//
//  GingerBreadMan.h
//

#ifndef GingerBreadMan_h
#define GingerBreadMan_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "MJMainGBase.h"
#include "GingerBreadMan_Item.h"
#include "PhonicInfoModel.h"
#include "GingerBreadMan_Board.h"
using namespace cocos2d;
class GingerBreadMan : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public GingerBreadMan_ItemDelegate, public GingerBreadMan_BoardDelegate
{
public:
    CREATE_FUNC(GingerBreadMan);

    struct GBM_INFO {
        std::string text;
        std::string audio_path_normal;
        std::string audio_path_slow;
        std::vector<std::string> audio_path_phonic;
        std::string image_path;
        std::vector<GingerBreadMan_Item::GBM_CHARACTER_INFO> list_character;
        std::vector<GingerBreadMan_Item::GBM_CHARACTER_INFO> list_distracting;

    };
    
    static GingerBreadMan *createGame(std::string json_file);
    
private:
    bool _spellStatus = false;
    std::string _audioIntro = "";
	void initHighlightIndexs(GBM_INFO & info);
    void didLoadFromCSB(std::string json_file);
    void generateData(rapidjson::Value value_data);
    std::vector<GBM_INFO> list_item_data;
    void onEnter() override;
    void onExitTransitionDidStar();
    
    GingerBreadMan_Board *board;
    
    void showIntroGame();
    cocos2d::Vec2 pos_guide;
    void loadFrames();
    cocos2d::ui::Layout *pie_layout, *gachngang_layout;
    int index_item = 0, index_charater = 0;
    void showPie();
    GingerBreadMan_Item::GBM_CHARACTER_INFO current_character;
    GBM_INFO current_item_info;
    cocos2d::ui::ImageView *box;
    std::vector<cocos2d::ui::ImageView *> list_gachngang;
    cocos2d::ui::ImageView *table;
    std::vector<GingerBreadMan_Item *> list_pie;
    void onItemClicked(cocos2d::Ref *sender) override;
    cocos2d::ui::Text *text_lb = nullptr;
    void showText();
    std::vector<Vec2> list_pos_letter, list_pos_letter_goal;
    std::vector<cocos2d::Sprite *> list_letter, list_letter_save;
    void runEffectGachNgang(std::vector<int> indexs);
    void onChoosePieRight(GingerBreadMan_Item *pie);
    void onShowLetter();
    void onNextTurnPlay();
    void onNextTurnPie();
    void onMergeLetter();
	void playSyncText();
    void playSlowText();
    void playNormalText();
    void setColorForText(Color3B color_text);
    void showImage();
    GAFWrapper *banhgung;
    void onSpeakerClicked(cocos2d::Ref *sender);
    std::string speaker_audio_path;
    std::vector <std::string> _speakerAudioPathWord;
    int index_right = 0;
    GAFWrapper *phaohoa3, *phaohoa1, *phaohoa2;
    cocos2d::ui::Button *speaker;
    cocos2d::ui::Layout *touch_layout;
};
CREATE_READER(GingerBreadMan);
#endif /* MJ_GingerBreadMan_h */
