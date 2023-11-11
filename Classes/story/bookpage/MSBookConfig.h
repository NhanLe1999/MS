#ifndef __MSBookConfig__
#define __MSBookConfig__

#include <stdio.h>
#include "cocos2d.h"

using namespace cocos2d;
using namespace std;

class MSBookConfig {
public:
    static MSBookConfig * getInstance();
    static MSBookConfig * __instance;
    
    std::string getFontNameBook();
    int test_list_quiz_id = 0;
    std::vector<int> list_quiz_id;
};

#endif
