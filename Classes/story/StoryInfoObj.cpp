#include "StoryInfoObj.h"
#include "NativeBridge.h"
#include "JsonDump.h"
#include "HSLib.h"
StoryInfoObj::StoryInfoObj() {
}

StoryInfoObj::~StoryInfoObj() {
}

StoryInfoObj::StoryInfoObj(cocos2d::ValueMap mapInfo){
    name = mapInfo["name"].asString();
    mostpopular = mapInfo["words"].asInt();
    newest = mapInfo["id"].asInt();
    
    languageid = mapInfo["lang_id"].asInt();
    readinglevel = mapInfo["level"].asInt();
    storyid = mapInfo["story_id"].asInt();
    
    lexile = mapInfo["lexile"].asString();
    word_count = mapInfo["words"].asInt();
    
    downloaded = mapInfo["downloaded"].asBool();
    total_page = mapInfo["total_page"].asInt();
    app_id = mapInfo["app_id"].asInt();
    _id = mapInfo["id"].asInt();
    thumbnail = mapInfo["thumbnail"].asString();
    written = mapInfo["written"].asString();
    design = mapInfo["design"].asString();
    trangbia = mapInfo["trangbia"].asString();
    
    image = mapInfo["image"].asString();
    _version = mapInfo["version"].asInt();
}
