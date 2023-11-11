#ifndef __TruyenTranh__NhanVatObj__
#define __TruyenTranh__NhanVatObj__
#include <stddef.h>
#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
using namespace std;
class NhanVatObj
{
public:
    NhanVatObj();
    explicit NhanVatObj(cocos2d::ValueMap map);
    //Object Data
    std::string fileName = "";
    cocos2d::Point posNV = cocos2d::Point(0,0);
    bool isGaf = false;
    bool istouch = true;
    int zorder = 1;
    int star_order = 100;
    int gaf_order = 0;
    cocos2d::Size contensize = cocos2d::Size(100, 100);
    
    cocos2d::ValueVector timeline;
    cocos2d::ValueVector listTextNV;
    std::string sound_effect = "";
    bool isEffectEnable = false; //ko load tu data
    bool is_reset_ani = true;
    bool is_run_with_mc = true;
    ~NhanVatObj();
};

#endif
