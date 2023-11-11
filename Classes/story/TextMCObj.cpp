#include "TextMCObj.h"

TextMCObj::TextMCObj() {
}

TextMCObj::TextMCObj(cocos2d::ValueVector vector, int typeStory){
    textMC = vector;
    box1 = textMC[0].asValueVector();
    countBox1 = (int)box1.size();
    type_story = typeStory;
    
    
//    namenv = map["name"].asString();
//    contensize = cocos2d::SizeFromString(map["contensize"].asString());
//    posNV = cocos2d::PointFromString(map["pos"].asString());
//    isGaf = map["isgaf"].asBool();
//    zorder = map["zorder"].asInt();
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
//
//    }
}

TextMCObj::~TextMCObj() {
}
