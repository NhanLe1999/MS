#include "NhanVatObj.h"

NhanVatObj::NhanVatObj() {
    fileName = "";
    contensize = cocos2d::Size(0,0);
    posNV = {0,0};
    isGaf = false;
    zorder = 1;
    isEffectEnable = false;
    sound_effect = "";
    star_order = 100;
}

NhanVatObj::NhanVatObj(cocos2d::ValueMap map){
    fileName = map["name"].asString();
    contensize = cocos2d::SizeFromString(map["contensize"].asString());
    posNV = cocos2d::PointFromString(map["pos"].asString());
    isGaf = map["isgaf"].asBool();
    zorder = map["zorder"].asInt();
    sound_effect = map["sound_effect"].asString();
    
    if (!map["star_order"].isNull()) {
        star_order = map["star_order"].asInt();
        if (star_order <= 0) {//Không set order = 0 -> chuyển về cuối hàng.
            star_order = 100;
        }
    }
    
    gaf_order = map["gaf_order"].asInt();
    
    if (!map["istouch"].isNull()) {
        istouch = map["istouch"].asBool();
    }else istouch = true;
    
    if (!map["textnv"].isNull() && map["textnv"].getType() == Value::Type::VECTOR) {
        listTextNV = map["textnv"].asValueVector();
    }
    
    if (!map["is_run_with_mc"].isNull()) {
        is_run_with_mc = map["is_run_with_mc"].asBool();
    }
    
    if (!map["is_reset_ani"].isNull()) {
        is_reset_ani = map["is_reset_ani"].asBool();
    }
    
//    star_postion = PointFromString(map["star_postion"].asString());

    //Text nhân vật trả về mảng nên bỏ phần này!
//    if (!map["textnv"].isNull()) {
//        auto mapText = map["textnv"].asValueMap();
//        text = mapText["text"].asString();
//        widthText = mapText["width"].asFloat();
//        timeText = mapText["time"].asFloat();
//        posText = cocos2d::PointFromString(mapText["postext"].asString());
//        if (!mapText["timeline"].isNull()) {
//            timeline = mapText["timeline"].asValueVector();
//        }
//        soundName = mapText["audio"].asString();
//    }
}

NhanVatObj::~NhanVatObj() {
}
