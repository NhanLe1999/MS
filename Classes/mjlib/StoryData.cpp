//
//  StoryData.cpp
//  MonkeyJunior
//
//  Created by developer on 2/29/16.
//
//

#include "StoryData.h"
#include "GlobalFunc.h"
StoryData* StoryData::_instance = NULL;

StoryData* StoryData::getInstance()
{
    if (_instance == nullptr) {
        _instance = new StoryData();
        
    }
    return _instance;
}

