
#include "GameWaterSpray_Item.h"
#include "HSAudioEngine.h"
#define SOUND_CORRECT_ANSWER "sounds/correct.mp3"

INITIALIZE_READER(MJGameWaterSpray_Item);

USING_NS_CC;
#define CSB_NAME "csb/game/waterspray/GameWaterSpray_Item.csb"

MJGameWaterSpray_Item * MJGameWaterSpray_Item::createGame(ItemInfo item_info, int type) {
    auto p = static_cast<MJGameWaterSpray_Item *>(cocos2d::CSLoader::createNodeWithVisibleSize(CSB_NAME));
    if (p) {
        p->didLoadFromCSB(item_info, type);
        return p;
    }

    CC_SAFE_DELETE(p);
    return nullptr;
}

void MJGameWaterSpray_Item::didLoadFromCSB(ItemInfo item_info, int type) {
    _item_info = item_info;
    type_item = type;
    if (type_item == TEXT_TYPE) {
        auto text = cocos2d::utils::findChild<cocos2d::ui::Text *>(this, "lb_item");
        cocos2d::utils::findChild(this, "img_item")->setVisible(false);
        text->setString(item_info.text);
    }else{
        auto img = cocos2d::utils::findChild<cocos2d::ui::ImageView *>(this, "img_item");
        cocos2d::utils::findChild(this, "lb_item")->setVisible(false);
        img->loadTexture(item_info.img_path);

    }
}

void MJGameWaterSpray_Item::onEnter(){
    cocos2d::ui::Layout::onEnter();
}

void MJGameWaterSpray_Item::onExit()
{
    CCLOG("thoat cmnr");
    cocos2d::ui::Layout::onExit();
}

void MJGameWaterSpray_Item::onRightAnswer(string rightAnswer){
    CCLOG("onWrightAnswer");
    auto duration = 0.1f;
    is_click_enable = false;
    auto layoutRight = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "layout_wrong");
    layoutRight->setBackGroundColor(Color3B::GREEN);
    layoutRight->setVisible(true);
    AUDIO_ENGINE->play(SOUND_CORRECT_ANSWER, false, 1, [=]() {
        this->runAction(
            Sequence::create(ScaleTo::create(duration, 1.1f),
                ScaleTo::create(duration, 1.0f),
                ScaleTo::create(duration, 1.1f),
                ScaleTo::create(duration, 1.0f),
                CallFunc::create([=] {
                layoutRight->setVisible(false);
                //is_click_enable = false;
                    }),
            NULL));

        AUDIO_ENGINE->play(rightAnswer, false, 1, [=]() {});
    });
	

    
    //auto bg_item = this->getChildByName("bg_item");
    //auto tick = GAFWrapper::createGAF("gaf/waterspray/tick/tick.gaf");
    //bg_item->addChild(tick, 10000);
    //tick->setPosition(bg_item->getContentSize()/2);
    //tick->play(false);
}

void MJGameWaterSpray_Item::onWrongAnswer(string rightAnswer){
    CCLOG("onWrongAnswer");
    auto duration = 0.1f;
    auto height = 10;
    is_click_enable = false;
    this->isTouchEnabled();

    auto layout_wrong = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "layout_wrong");
    layout_wrong->setBackGroundColor(Color3B::RED);
    layout_wrong->setVisible(true);

    AUDIO_ENGINE->play("sounds/waterspray/waterspray_wrong.mp3", false, 1, [=]() {
            this->runAction(
                Sequence::create(
                    MoveBy::create(duration, Vec2(0, height)),
                    MoveBy::create(duration, Vec2(0, -height)),
                    MoveBy::create(duration, Vec2(0, height)),
                    MoveBy::create(duration, Vec2(0, -height)),
                    CallFunc::create([=] {
                        layout_wrong->setVisible(false);
                        }),
                    NULL));
            AUDIO_ENGINE->play("sounds/waterspray/waterspray_mc2.mp3", false, 1, [=]() {
                AUDIO_ENGINE->play(rightAnswer, false, 1, [=]() {
                    is_click_enable = true;

                    });
                });
        });
    
    
    
}

int MJGameWaterSpray_Item::getClickEnable()
{
    return is_click_enable;
}

void MJGameWaterSpray_Item::setClickEnable(bool click_enable)
{
    is_click_enable = click_enable;
}

void MJGameWaterSpray_Item::setDelegate(MJGameWaterSpray_ItemDelegate *_delegate){
    this->delegate = _delegate;
}


void MJGameWaterSpray_Item::onChoose(cocos2d::Ref *sender){
    if (!is_click_enable) return;
    if (delegate) {
        delegate->onClickItem(this, type_item);
    }
}

ui::Widget::ccWidgetClickCallback MJGameWaterSpray_Item::onLocateClickCallback(const std::string &callback_name) {
    if (callback_name == "onChoose") {
        return CC_CALLBACK_1(MJGameWaterSpray_Item::onChoose, this);
    }
    return nullptr;
}
