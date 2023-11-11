//
//  StoryTheEndPage.h
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#ifndef __StoryTheEndPage__
#define __StoryTheEndPage__

#include "CocoStudioMacro.h"
#include "ui/CocosGUI.h"
#include "../cocos2d/external/gaf/Library/Sources/GAF.h"
#include "HSBaseLayout.h"
#include "GAFWrapper.h"
using namespace cocos2d;
using namespace cocos2d::ui;
class StoryTheEndPage : public HSBaseLayout,
public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    CREATE_FUNC(StoryTheEndPage);
    static StoryTheEndPage * createPage(int type, bool isHaveGame);
    void runAnimShow();
    
private:
    void preload(int type, bool isHaveGame);
    void didLoadFromCSB();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    void onEnter() override;
    void onEnterTransitionDidFinish() override;
    void onExitTransitionDidStart() override;
    bool isShow;
    int endingpage_type = 1;
    bool _isHaveGame;
    std::vector<gaf::GAFObject *> list_face_rate;
    gaf::GAFObject *taungam;
    void callbackRequestUrl(cocos2d::ValueMap map_response, int tagRequest) override;
    void showYouHaveLearn();
    std::vector<Label *> list_text;
    std::vector<ui::Scale9Sprite *> list_khucgo;
    int _type = 1;
    enum YouHaveLearn_Type {
        KeyWord_Type = 1,
        Structure_Type = 2,
        Other_Type = 3,
    };
    
    void addTextKeyWithList(ValueVector list_key);
    void showBtPlay();
    void showTextWithAnim(ui::Scale9Sprite *khucgo);
    Text *title;
    std::string path_audio, path_audio_other;
    void showAnimKeyWord();
    void addTextOtherCategory(std::string other_str);
    void showAnimOtherCategory();
    std::vector <std::string> list_str;
    vector<string> explode(const string& str, const char& ch);
    void updateQuesLb(float time);
    int count_str = 0;
    
    void onSkip(Ref *sender);
    void onPlayGame(Ref *sender);
    
    
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
    void updateClipShowView(float dt);
    float size_h = 0, size_w = 0;
    Node *line_run;
   
    enum ENDINGPAGE_TYPE {
        ENDPAGE_TYPE = 1,
        ENDGAME_TYPE = 2,
    };
    void showViewWhenEndGame();
    void showViewWhenEndPage();
    
    void showFireWork();
    void showRecomentStories();
    
    GAFWrapper *phao_giay, *phao_trai, *phao_phai, *phao_hoa, *monkey_dance;
public:
//    void showAlertView(int index);

};

CREATE_READER_CLASS(StoryTheEndPageReader, StoryTheEndPage);

#endif /* defined(__ALTP__StoryTheEndPage__) */
