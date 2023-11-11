#ifndef __NhanVat__
#define __NhanVat__

#include <stdio.h>
#include "cocos2d.h"
#include "../cocos2d/external/gaf/Library/Sources/GAF.h"
#include "ui/CocosGUI.h"
#include "HSLib.h"
#include "NhanVatObj.h"
#include "TextNVObj.h"

using namespace cocos2d;
using namespace std;
class NhanVat: public cocos2d::ui::Layout {
public:
    virtual bool init();
    CREATE_FUNC(NhanVat);
    gaf::GAFObject *gaf_object = nullptr;
    static NhanVat * createOne(std::string fileName, bool loop, NhanVatObj nhanvatobj);
    bool initOne(std::string fileName, bool loop, NhanVatObj nhanvatobj);
    //cocos2d::ui::Layout *panel;
    NhanVatObj nvObj;
    TextNVObj _textNVObj;
    void displayTextNhanvat(Vec2 _posText, int touch_index);
    bool isRunning = false, isDisplayText = false;
    void onFinishAction(gaf::GAFObject *obj);
    void startAction(bool isLoop, Vec2 _posText, int touch_index);
    void startEffect();
    bool isFirstTouch = false;
    void testText(Vec2 _posText);
    void playGAF();
    void pauseStarEffect();
    void resumStarEffect();
    void stopAllActionAndGaf();
    std::vector<Layout *> _listTouchPanel;
    std::vector<gaf::GAFObject *> _listGuideTouch;
    bool init_gaf_done = true;
private:
    Color3B normal_color;
    Color3B highlight_color;
    void highlightTextAction();
    int highlight_index = 0;
    int iStart = 0;
    int iEnd = 1;
    ParticleSystemQuad *exploisive;
    bool is_click_pause_ani_gaf = false;
//    gaf::GAFObject *guide_touch;
    
};

#endif
