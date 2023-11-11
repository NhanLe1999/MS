#ifndef __BISCUIT_OBJECT_H__
#define __BISCUIT_OBJECT_H__

#include "cocos2d.h"
#include "AspectR.h"
#include "ui/CocosGUI.h"
#include "ItemBack.h"

using namespace cocos2d;
using namespace std;
using namespace cocos2d::ui;

class ItemFront : public Layout
{
private:
    ImageView* _image = nullptr;
    Label* _caption = nullptr;
    void initOptions(string letter, int type, Size size);
    
public:
    ItemBack* holder = nullptr;
    Vec2 originalPos;
    bool placed = false;
    bool hided = false;

    ItemFront();
    ~ItemFront();
    static ItemFront* create(const string letter, int type, Size size);
    Label* caption();
    void fadeOut();
};

#endif // __LETTERTILE_OBJECT_H__
