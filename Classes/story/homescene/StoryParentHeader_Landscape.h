//
//  StoryParentHeader_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/10/17.
//
//

#ifndef StoryParentHeader_Landscape_hpp
#define StoryParentHeader_Landscape_hpp

#include "CocoStudioMacro.h"

class StoryParentHeader_Landscape : public cocos2d::ui::Layout {
public:
    CREATE_FUNC(StoryParentHeader_Landscape);
    static StoryParentHeader_Landscape * createHeader(const std::string & header);
    static StoryParentHeader_Landscape * createHeader(const std::string & header, const std::string & deviceID);

    void adjustWidth(float width);
};

CREATE_READER_CLASS(StoryParentHeader_LandscapeReader, StoryParentHeader_Landscape);

#endif /* StoryParentHeader_Landscape_hpp */
