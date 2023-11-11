
#ifndef MsHelperGame_hpp
#define MsHelperGame_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "platform/CCPlatformConfig.h"
#include "../external/json/rapidjson.h"
#include "../external/json/document.h"
#include "../external/json/prettywriter.h"
#include "../external/json/istreamwrapper.h"

#include "Math_Libs.h"
#include "AudioEngine.h"

namespace ms {

    namespace game {
    
        void startHighlightText(std::vector<math::resource::Sync_Text> listTextHight, cocos2d::ui::Text* text, std::string audioPath = "",std::function<void(int au_id, std::string path)> callback = nullptr,  std::string answer = "", int spaceBetweenWords = 10, cocos2d::Color4B startSyncTextColor = cocos2d::Color4B::RED, cocos2d::Color4B endSyncTextColor = cocos2d::Color4B::BLACK);
    
        std::vector<std::pair<int, int>> getTextIndexSentence(std::string text, std::vector<math::resource::Sync_Text> syncDatas);
    
        void highLightText(cocos2d::ui::Text* text, cocos2d::Node* currentNode, cocos2d::Color4B textOriginColor, cocos2d::Color3B highlightCorlor, int index, std::vector<math::resource::Sync_Text> syncDatas, std::vector<std::pair<int, int>> vectorTextIndex, std::function<void(int,std::string)> callback);
        void startHighlightText1(std::vector<math::resource::Sync_Text> listTextHight, cocos2d::ui::Text* text, std::string audioPath = "",std::function<void(int au_id, std::string path)> callback = nullptr,  std::string answer = "", int spaceBetweenWords = 10, cocos2d::Color4B startSyncTextColor = cocos2d::Color4B::RED, cocos2d::Color4B endSyncTextColor = cocos2d::Color4B::BLACK);
        
    }
}

#endif
