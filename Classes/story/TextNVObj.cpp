#include "TextNVObj.h"

TextNVObj::TextNVObj() {
    text = "";
    widthText = 0;
    timeText = 0;
    posText = {0,0};
    soundName = "";
}

TextNVObj::TextNVObj(cocos2d::ValueMap map){
    text = map["text"].asString();
    widthText = map["width"].asFloat();
    timeText = map["time"].asFloat();
    posText = cocos2d::PointFromString(map["postext"].asString());
    soundName = map["audio"].asString();
    touchboundingbox = RectFromString(map["touchboundingbox"].asString());
    star_postion = PointFromString(map["star_postion"].asString());
}

TextNVObj::~TextNVObj() {
}
