//
//  MSTrialPopup.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/7/17.
//

#include "MSTrialPopup.h"

USING_NS_CC;
INITIALIZE_READER(MSTrialPopup);

std::once_flag ms_trial_popup_reader;

MSTrialPopup * MSTrialPopup::createPopup() {
    std::call_once(ms_trial_popup_reader, [] {
        REGISTER_CSB_READER(MSTrialPopup);
    });
    
    auto popup = reinterpret_cast<MSTrialPopup *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/TrialPopup.csb"));
    if (popup) {
        popup->didLoadFromCSB();
        return popup;
    }
    
    CC_SAFE_DELETE(popup);
    return nullptr;
}

void MSTrialPopup::didLoadFromCSB() {
    
}
