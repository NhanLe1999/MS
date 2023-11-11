//
//  MJWorksheetNoResult.h
//  MJStory
//
//  Created by Hieu Nguyen on 12/5/20.
//

#ifndef MJWorksheetNoResult_h
#define MJWorksheetNoResult_h

#include <stdio.h>
#include "cocostudiomacro2.h"

USING_NS_CC;

class MJWorksheetNoResult : public cocos2d::ui::Layout
{
public:
    CREATE_FUNC(MJWorksheetNoResult);
    static MJWorksheetNoResult* createView(const std::string& keyword, std::string nameGrade = "");
    
    void setKeyword(const std::string& keyword);
    void setBackgroundColor(const Color3B& color);
    
private:
    void didLoadFromCSB(const std::string& keyword, std::string nameGrade);
    float setPositionForStart(std::string nameStar, std::string textStar);
    bool _hideText = false;
};

CREATE_READER(MJWorksheetNoResult);
#endif /* MJWorksheetNoResult_h */
