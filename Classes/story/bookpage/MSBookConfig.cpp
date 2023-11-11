#include "MSBookConfig.h"
#include "HSLib.h"
MSBookConfig * MSBookConfig::__instance = NULL;
MSBookConfig * MSBookConfig::getInstance() {
    if (!__instance) {
        __instance = new MSBookConfig();
    }
    return __instance;
}

std::string MSBookConfig::getFontNameBook(){
    return XHSLib->fontNameStory;
}

