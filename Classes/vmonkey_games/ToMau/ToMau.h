#ifndef ToMau_HPP
#define ToMau_HPP

#include "cocostudiomacro2.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "GAFWrapper.h"
#include "MJMainGBase.h"
#include "GameData.h"

#include "Math_Object.h"
#include "StoryAlert.h"
//#include "PhonicInfoModel.h"
USING_NS_CC;
using namespace ui;

class ToMau : public MJMainGBase, public cocostudio::WidgetCallBackHandlerProtocol, public StoryAlertDelegate{
public:
    CREATE_FUNC(ToMau);
    static ToMau * createGame();
    void generateData();
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;
    cocos2d::ui::Widget::ccWidgetTouchCallback onLocateTouchCallback(const std::string &callback_name) override;
private:
    void didLoadFromCSB();
    //data
    
    mj::GameObject question;
    
    void loadFrames();
    void onEnter() override;
    void onExitTransitionDidStart() override;
    void onFinishGame() override;
    void inActiveMode(float delay_time = 5.0f);
    
    void showTapGuideGame(Vec2 pos1, Vec2 pos2, float delay_time, std::string gaf_guide_path = "gaf/guidegame/hand2/hand2.gaf");
    void hideTapGuideGame();
    
    void showBottomColorPad(std::vector<std::string> colors, math::callback::onTouchObjectBegan callback, math::callback::onAnswerClicked callback_done);
    void showRightColorPad(std::vector<std::string> colors, math::callback::onTouchObjectBegan callback, math::callback::onAnswerClicked callback_done);

    void loadColor();
    void onTouchObject(Ref* sender, TouchEventType type);
    void onTouchPanel(Ref* sender, TouchEventType type);
    void onClickSpeaker(Ref* sender);
    void selectPen(Ref* sender);
    void onNext(Ref* sender);
    void onAlertOK(cocos2d::Ref * sender) override;
    void onAlertCancel(cocos2d::Ref * sender) override;
    
    Layout* root_layout;
    Layout* img_holder;
    Layout* color_panel;
    int audio_id_question;
    std::vector<std::string> selected_color;
    std::string paper_img;
    std::vector<math::resource::Image*> list_color;
    cocos2d::ui::ImageView* _color_selected;
    std::vector<Math_Object*> _color_pens;
    std::string current_color = "";
    std::map<std::string, Color3B> colors_map;
    
    std::map<ImageView*, Image*> objects_list;
    std::vector<ImageView*> objects_order;
    bool is_free = true;
    bool is_portrait = false;
    Vec2 pos_character;
    std::string font_name = "fonts/Montserrat-Medium.ttf";
    int font_sz = 70;

    Vec2 touch_pos = Vec2(0, 0);

};

CREATE_READER(ToMau);

#endif
