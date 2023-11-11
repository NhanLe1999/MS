
#ifndef MSBookObject_hpp
#define MSBookObject_hpp

#include "cocostudiomacro2.h"
#include "MSBookData.h"
#include "ui/CocosGUI.h"
#include "GAFWrapper.h"
#define OBJECT_TYPE_IMAGE "image"
#define OBJECT_TYPE_ANIMATION "animation"
#define OBJECT_TYPE_LAYOUT "layout"

#define ANI_TYPE_BEGIN "begin"
#define ANI_TYPE_END "end"
#define ANI_TYPE_SEQUENCE "sequence"
using namespace cocos2d;
using namespace ui;

class LayoutPolygon : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(LayoutPolygon);
    CC_SYNTHESIZE(std::vector<cocos2d::Point>, _vertices, Vertices);
};

class MSBookObjectDelegate {
public:
    virtual void onTouchObject(std::string _content, float _wtext, Vec2 _posText, float _duration,mj::BOOK_IMAGE_INFO i_info) {};
    virtual void onRunAniObjectWithMCDone(){};
    virtual void onActionGuideEffectObject(std::string action){};
    virtual void onRunAniWithSequenceDone(){};
};

class MSBookObject : public cocos2d::ui::Layout, MSBookObjectDelegate, public cocostudio::WidgetCallBackHandlerProtocol  {
public:
    CREATE_FUNC(MSBookObject);
    static MSBookObject * createObject(mj::BOOK_IMAGE_INFO book_image_info);
    mj::BOOK_IMAGE_INFO _book_image_info;
    bool touch_enable = false;
    void startPlaying();
    void setDelegate(MSBookObjectDelegate *delegate);
    void runAniWithMC();
    void startEffect();
    bool isFirstTouch = false;
    void pauseStarEffect();
    void resumStarEffect();
    bool isEffectEnable = false;
    void stopAllActionAndEffect();
    bool check_have_star = false;
    bool init_gaf_done = true;
    std::vector<LayoutPolygon *> list_touch_box;
    void playAudioObject(cocos2d::Vec2 pos_touch);
    GAFWrapper *getAnimation() { return animation;}
private:
    void didLoadFromCSB(mj::BOOK_IMAGE_INFO book_image_info);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &callback_name) override;

    void onRefresh(cocos2d::Ref * sender);
    void onEnter() override;
    void onExitTransitionDidStart() override;
    void addObject();
    void addImage();
    void addAnimation();
    void addTouchBox();
    GAFWrapper *animation = nullptr;
    
    
    bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event) override;
    void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event) override;
    Vec2 touch_pos, touch_end;
    MSBookObjectDelegate *_delegate = nullptr;
    
    void runAniWithTouch();
    void runAniWithSequence();
    void runAniWithFirst();
    bool is_running_ani = false;
    std::vector<GAFWrapper *> _listGuideTouch;
    

};

CREATE_READER(MSBookObject);

#endif
