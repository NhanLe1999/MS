#ifndef __TruyenTranh__StoryInfoObj__
#define __TruyenTranh__StoryInfoObj__
#include <stddef.h>
#include <iostream>
#include "cocos2d.h"
#include "MJStoryInfoModel.h"
using namespace cocos2d;
using namespace std;
class StoryInfoObj
{
public:
    StoryInfoObj();
    explicit StoryInfoObj(cocos2d::ValueMap mapInfo);
    //Object Data
    mj::model::ActInfo::Type typeStory = mj::model::ActInfo::Type::STORY;
    string name;
    int mostpopular = 0, newest = 0, readinglevel = 1, languageid = 1, storyid = 0, total_page = 0, app_id = 0;
    int word_count = 0;
    string lexile;
    int _id = 0, _storyid = 0, _version = 0;
    bool downloaded = false;
    std::string thumbnail, written, design, trangbia, image;
    ~StoryInfoObj();
};

#endif
