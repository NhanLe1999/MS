#ifndef __BISCUITBACK_OBJECT_H__
#define __BISCUITBACK_OBJECT_H__

#include "cocos2d.h"
#include "AspectR.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

class ItemBack : public Layout
{
private:
    ImageView* _image = nullptr;
    //Label* _caption;
    void initOptions(string letter, int type, Size size);
    
public:
    ItemBack();
    ~ItemBack();
    static ItemBack* create(const string letter, int type, Size size);
    bool isOverlap(Rect otherRect);
    void fadeOut();
    void onTouchItem(Ref* sender, TouchEventType type );
};

#endif // __LETTERTILE_OBJECT_H__
