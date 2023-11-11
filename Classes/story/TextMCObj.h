#ifndef __TruyenTranh__TextMCObj__
#define __TruyenTranh__TextMCObj__
#include <stddef.h>
#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class TextMCObj
{
public:
    TextMCObj();
    explicit TextMCObj(cocos2d::ValueVector vector, int typeStory);
    //Object Data
    ValueVector textMC;
    ValueVector box1;
    int type_story = 1;
    int countBox1 = 0;
    ~TextMCObj();
};

#endif
