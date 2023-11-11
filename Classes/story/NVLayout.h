#ifndef __NVLayout__
#define __NVLayout__

#include <stdio.h>
#include "cocos2d.h"
#include "../cocos2d/external/gaf/Library/Sources/GAF.h"
#include "ui/CocosGUI.h"
#include "HSLib.h"
#include "NhanVatObj.h"
using namespace cocos2d;
using namespace std;
class NVLayout: public cocos2d::ui::Layout {
public:
    virtual bool init();
    CREATE_FUNC(NVLayout);
    static NVLayout * createOne(NhanVatObj nvObj);
    bool initOne(NhanVatObj NhanVatObj);
    cocos2d::ui::Layout *panel;
    NhanVatObj nvObj;
    void displayTextNVLayout();
    bool isDisplayText = false;
    void startAction(bool isLoop);
private:
    Color3B normal_color;
    Color3B highlight_color;
    void highlightTextAction();
    int highlight_index = 0;
    int iStart = 0;
    int iEnd = 1;

};

#endif
