//
// Created by Pijush Sarkar on 13/11/17.
//

#ifndef buildword_LEVELDATA_H
#define buildword_LEVELDATA_H
#include "cocos2d.h"

using namespace std;

namespace buildword {
    class LevelData {
    public:
        string word;
        string wordAudio;
        std::vector<std::string> phonic_list;
        unordered_map<string,string> charAudio;
    };
}




#endif //PROJ_ANDROID_STUDIO_LEVELDATA_H
