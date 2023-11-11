#ifndef __TruyenTranh__TextNVObj__
#define __TruyenTranh__TextNVObj__
#include <stddef.h>
#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class TextNVObj
{
public:
    TextNVObj();
    explicit TextNVObj(cocos2d::ValueMap map);
    //Object Data
    std::string text = "";
    cocos2d::Point posText = cocos2d::Point(0,0);
    float widthText = 0;
    float timeText = 0;
    
    std::string soundName = "";
    Rect touchboundingbox = Rect(0, 0, 0, 0);
    Point star_postion = Point(0,0);
    ~TextNVObj();
};

#endif
