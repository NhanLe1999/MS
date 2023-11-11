//
// Created by Pijush Sarkar on 13/11/17.
//

#ifndef houseofwordsDATA_H
#define houseofwordsDATA_H
#include "cocos2d.h"

using namespace std;
namespace houseofwords {
    class LevelData {
    public:
        string ques;
        string quesAudio;
        string text_info;
        string infoAudio;
        std::vector<std::vector<string>> options;
        int answer;
        string mainImage;
        string charImage;
    };
}




#endif //PROJ_ANDROID_STUDIO_LEVELDATA_H
