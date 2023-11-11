#include "NVLayout.h"
#include "mjlib/Common.h"
#include "HSLib.h"
USING_NS_CC;
//gaf::GAFObject *gaf_object = nullptr;
bool NVLayout::init() {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    };
    return true;
}

#pragma mark - PLAY ANI PET - One layer
bool NVLayout::initOne(NhanVatObj nvObj) {
    if (!cocos2d::ui::Layout::init()) {
        return false;
    };
    
    this->nvObj = nvObj;
    panel = cocos2d::ui::Layout::create();
    panel->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
    panel->setBackGroundColor(cocos2d::Color3B(random(0, 255), random(0, 255), random(0, 255)));
    panel->setOpacity(0);
    panel->setContentSize(nvObj.contensize);
    this->addChild(panel);
    panel->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
    normal_color = Color3B::BLACK;
    highlight_color = Color3B::RED;
    return true;
}

NVLayout * NVLayout::createOne(NhanVatObj nvObj) {
    auto p_food = new NVLayout();
    if (p_food && p_food->initOne(nvObj)) {
        p_food->autorelease();
    } else {
        delete p_food;
        p_food = nullptr;
    }
    return p_food;
}


void NVLayout::displayTextNVLayout(){
}

void NVLayout::startAction(bool isLoop){
    if (isDisplayText) return;
       displayTextNVLayout();
    if (!isLoop) {
        isDisplayText = true;
    }
}

void NVLayout::highlightTextAction(){
    this->unscheduleAllCallbacks();
    auto bgText = this->getChildByName("bgtextNhanvat");
    if (!bgText) return;
    auto label = bgText->getChildByName<Label *>("textNhanvat");
    if (!label) return;
    if((nvObj.timeline.size()) == highlight_index) {
        label->setColor(normal_color);
        highlight_index = 0;
        return;
    }
    
    ValueMap map = nvObj.timeline[highlight_index++].asValueMap();
    iStart = map["ts"].asInt();
    iEnd = map["te"].asInt();
    label->setColor(normal_color);
    for(int i = iStart; i<= iEnd; i++) {
        auto letter = label->getLetter(i);
        if(letter != NULL) {
            letter->setColor(Color3B::RED);
        }
    }
    
    auto duration = (map["e"].asFloat() - map["s"].asFloat())/1000.0f;
    this->scheduleOnce([=](float dt){
        highlightTextAction();
    }, duration, StringUtils::format("highlight_index_%i", highlight_index));
}
