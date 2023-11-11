//
//  CompletePicture.hpp
//  mj_gamelib
//
//  Created by AnhB Nguyen on 8/21/18.
//

#ifndef CompletePicture_hpp
#define CompletePicture_hpp

#include "MJMainGBase.h"
#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJHelper.h"
#include "../external/gaf/Library/Sources/GAFAsset.h"
//#include "Math_Libs.h"
#include "GameData.h"
#include "StoryAlert.h"
USING_NS_CC;
using namespace ui;

class CompletePicture : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate  {
public:
    CREATE_FUNC(CompletePicture);
    static CompletePicture * createGame();
    
private:
    void didLoadFromCSB();

    void generateData();
    void onEnter() override;
    void loadFrame();
    void onFinishGameVMonkey() override;
    void onExitTransitionDidStart() override;
 
    Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    
     Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string &callback_name) override;
    
    void onTouchObject(Ref* sender, TouchEventType type );
    void onClickSpeaker(Ref* sender);
    void onTouchPicture();
    void onShare(Ref* sender);
    void onNext(Ref* sender);
    
    void onComplete(std::function<void()> callback = nullptr);
    void readAnswer(int count);
    
    bool checkAllInboxObject();
    void showDoneButton();
    
     mj::GameObject qo;
    Layout* root_layout;
    Button* share_btn, *next_btn;
    ImageView* bg_img = nullptr;
    bool is_free = true;
    bool img_zoom = false;
    int number_obj = 0;
    int count = 0;
    std::map<std::string, Color3B> _colors;
    std::vector<std::string> answer_audios;
    Vec2 pos;
    Size screen_size;
    std::string font_name = "fonts/Montserrat-Medium.ttf";
    int font_sz = 35;
    bool autoPlay = false;
    int audio_id_question = -1;
    bool isDone = false;
    void onAlertOK(cocos2d::Ref * sender) override;
    bool is_mattroi_act = false;
    std::string have_eye = "";
    std::string have_mouth = "";
    
    
};
CREATE_READER(CompletePicture);

#endif /* CompletePicture_hpp */
