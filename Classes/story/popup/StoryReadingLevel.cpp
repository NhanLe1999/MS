//
//  StoryReadingLevel.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryReadingLevel.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "NativeBridge.h"
#include "StoryDataModel.h"
#define text_marin 50
INITIALIZE_READER_INSTANCE(StoryReadingLevelReader);

USING_NS_CC;
using namespace ui;

StoryReadingLevel * StoryReadingLevel::createPage() {
    auto p = static_cast<StoryReadingLevel *>(CSLoader::createNodeWithVisibleSize("csb/StoryReadingLevel.csb"));
    if (p) {
        p->didLoadFromCSB();
    }
    return p;
}


void StoryReadingLevel::didLoadFromCSB() {
    classes_name = "levelinfo";
}
#pragma mark - onEnter
void StoryReadingLevel::onEnter() {
    Layout::onEnter();
    if (MJDEFAULT->getBoolForKey(key_readinglevel_next)) {
        isNextClicked = true;
        Node *layout_bg;
        if (_isPortrait) {
            layout_bg = this->getChildByName("layout_doc");
        }else{
            layout_bg = this->getChildByName("layout_ngang");
        }
        layout_bg->setVisible(true);
        //displayReadingLevel();
    }else displayWelcome();
}

void StoryReadingLevel::displayWelcome(){
    auto layout_welcome = this->getChildByName("layout_welcome");
    layout_welcome->setVisible(true);
    Node *bg;
    if (_isPortrait) {
        bg = layout_welcome->getChildByName("welcome_doc");
    }else bg = layout_welcome->getChildByName("welcome_ngang");
    bg->setVisible(true);
    welcome_titlelb = bg->getChildByName<Text *>("title_lb");
    auto contentlb = bg->getChildByName<Text *>("content_lb");
    welcome_contentlb = Label::createWithTTF("Reading Level System", "fonts/monkeystories_thin.ttf", 30);
    welcome_contentlb->setWidth(bg->getContentSize().width*0.8);
    welcome_contentlb->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    welcome_contentlb->setAnchorPoint(contentlb->getAnchorPoint());
    welcome_contentlb->setPosition(contentlb->getPosition());
    welcome_contentlb->setTextColor(Color4B::BLACK);
    contentlb->setVisible(false);
    bg->addChild(welcome_contentlb);
    
    welcome_titlelb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "welcome_title"));
    welcome_contentlb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "welcome_content"));
    static_cast<Button *>(ui::Helper::seekWidgetByName(this, "btnext"))->setTitleText(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "btnext"));
}

void StoryReadingLevel::displayReadingLevel(){
        list_level_des = StoryDataModel::getInstance()->getAllLevelInfo(MJDEFAULT->getIntegerForKey(key_current_lang, 1));
        if ((int)list_level_des.size() == 0) return;
        for (int i = 0; i < (int)list_level_des.size(); i++) {
            if (list_level_des[i].getType() != Value::Type::MAP) continue;
            auto map = list_level_des[i].asValueMap();
            list_levelid.push_back(Value(map["id"].asInt()));
            
//        }
    }
    
    std::reverse(list_level_des.begin(), list_level_des.end());
    std::reverse(list_levelid.begin(), list_levelid.end());

    Node *layout_bg;
    if (_isPortrait) {
        layout_bg = this->getChildByName("layout_doc");
    }else{
        layout_bg = this->getChildByName("layout_ngang");
    }
    layout_bg->setVisible(true);
    //    auto bg = layout_bg->getChildByName("bg");
    titlelb = layout_bg->getChildByName("titlebg")->getChildByName<Text *>("title");
    titlelb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "reading_title"));
    
    listview_level = layout_bg->getChildByName<ListView *>("listview_level");
    listview_level->setTouchEnabled(true);
    listview_level->setClippingEnabled(true);
    listview_level->setScrollBarEnabled(false);
    listview_level->setMagneticType(ListView::MagneticType::BOTH_END);
    scroll_content = layout_bg->getChildByName<ScrollView *>("scroll_content");
    scroll_content->setScrollBarEnabled(false);
    scroll_content->setClippingEnabled(true);

    content_level_lb = Label::createWithTTF("", "fonts/monkeystories_medium.ttf", 28);
    scroll_content->addChild(content_level_lb);
    content_level_lb->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    content_level_lb->setPosition(Vec2(text_marin,0));
    content_level_lb->setTextColor(Color4B::BLACK);
    if (_isPortrait) {
        content_level_lb->setRotation(-90);
        content_level_lb->setWidth(scroll_content->getContentSize().height-text_marin*2);
        
    }else {
        content_level_lb->setWidth(scroll_content->getContentSize().width-text_marin*2);
    }
    
    displayListLevel();
}


void StoryReadingLevel::initWithData(bool isPortrait){
    _isPortrait = isPortrait;
}

void StoryReadingLevel::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void StoryReadingLevel::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
}

#pragma mark - LOGIC_FUNTION
void StoryReadingLevel::displayListLevel(){
    float width = 0;
    for (int i = 0; i < (int) list_levelid.size(); i++) {
        Layout *bt;
        CCLOG("id = %d", list_levelid[i].asInt());
        if (list_levelid[i].asInt() == 1) {
            index_selected = i;
             bt = levelButtonWith(100+i, list_levelid[i].asInt(), true);
        }else {
            bt = levelButtonWith(100+i, list_levelid[i].asInt());
        }
        if (i == 0) {
            width = bt->getContentSize().height;
        }
        listview_level->addChild(bt);
    }
    listview_level->setInnerContainerSize(cocos2d::Size(listview_level->getContentSize().width, width*(int) list_levelid.size()));
    listview_level->jumpToBottom();
    displayContentOfLevel(list_levelid[index_selected].asInt());
}
void StoryReadingLevel::displayContentOfLevel(int level_id){
    auto map_level_info = list_level_des[index_selected].asValueMap();
//    content_level_lb->setString("Sáng nay mình em trước gương chợt nhớ một người em rất thương. Người đã khiến cuộc sống em khác đi rất nhiều. Và nếu như tình anh vẫn thế thì Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm, và yêu người ấy thật nhiều! Lời chia tay buồn đến mấy cũng không thể nào làm cho anh gục ngã đến mức tuyệt vọng, chỉ là vết thương sâu một chút thôi anh à. Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm, và yêu người ấy thật nhiều! Lời chia tay buồn đến mấy cũng không thể nào làm cho anh gục ngã đến mức tuyệt vọng, chỉ là vết thương sâu một chút thôi anh à. Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm. Sáng nay mình em trước gương chợt nhớ một người em rất thương. Người đã khiến cuộc sống em khác đi rất nhiều. Và nếu như tình anh vẫn thế thì Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm, và yêu người ấy thật nhiều! Lời chia tay buồn đến mấy cũng không thể nào làm cho anh gục ngã đến mức tuyệt vọng, chỉ là vết thương sâu một chút thôi anh à. Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm, và yêu người ấy thật nhiều! Lời chia tay buồn đến mấy cũng không thể nào làm cho anh gục ngã đến mức tuyệt vọng, chỉ là vết thương sâu một chút thôi anh à. Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm");
    CCLOG("level info = %s", map_level_info["des"].asString().c_str());
    content_level_lb->setString(map_level_info["des"].asString());
    auto text_height = content_level_lb->getContentSize().height;
    if (text_height > scroll_content->getContentSize().width) {
        scroll_content->setInnerContainerSize(cocos2d::Size(content_level_lb->getContentSize().height+text_marin/2, scroll_content->getContentSize().height));
    }else{
        scroll_content->setInnerContainerSize(scroll_content->getContentSize());
    }
    content_level_lb->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    content_level_lb->setPosition(Vec2(text_marin/2, scroll_content->getContentSize().height - text_marin));
    
}

Layout* StoryReadingLevel::levelButtonWith(int tag, int level_id, bool isSelect){
    auto layout = Layout::create();
    auto bt = Button::create("mjstory/popup/storyreadinglevel_item.png", "mjstory/popup/storyreadinglevel_item.png", "mjstory/popup/storyreadinglevel_itemchoose.png");
    layout->setContentSize(cocos2d::Size(bt->getContentSize().height, bt->getContentSize().width+0.75));
    bt->setRotation(-90);

    layout->addChild(bt);
    bt->setAnchorPoint(Vec2(0, 1));
    bt->setName("button");
    auto text = Text::create(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(general_lang, "level")+ " " + XHSLib->getNameLevelWithID(level_id), "fonts/monkeystories_medium.ttf", 17);
    bt->addChild(text);
    text->setName("label");
    text->setPosition(Vec2(bt->getContentSize().width/2, bt->getContentSize().height/2));

    if (isSelect) {
        bt->setPosition(Vec2(24, 0));
        text->setColor(Color3B(46,49,144));
        
        bt->setEnabled(false);
        bt->setBright(false);

    }else {
        bt->setPosition(Vec2(28, 0));
        text->setColor(Color3B(2,155,214));
    }
    text->setScaleX(0.95);
    bt->addClickEventListener(CC_CALLBACK_1(StoryReadingLevel::onChooseLevel, this));
    bt->setTag(tag);
    return layout;
}

void StoryReadingLevel::onChooseLevel(cocos2d::Ref *sender){
    auto tag = (int)static_cast<Button *>(sender)->getTag()-100;
    for (auto item : listview_level->getItems()) {
        auto layout = dynamic_cast<cocos2d::ui::Layout *>(item);
        if (!layout) return;
        auto button = layout->getChildByName<Button *>("button");
        if (!button) return;
        if ((button->getTag()-100) == tag) {
            button->setHighlighted(true);
            button->setPosition(Vec2(24, 0));
            
            auto text = button->getChildByName<cocos2d::ui::Text *>("label");
            text->setColor(Color3B(46,49,144));
            
            button->setEnabled(false);
            button->setBright(false);
            
        }else {
            button->setPosition(Vec2(28, 0));
            
            auto text = button->getChildByName<cocos2d::ui::Text *>("label");
            text->setColor(Color3B(2,155,214));
            
            button->setEnabled(true);
            button->setBright(true);
            
        }

    }
    index_selected = tag;
    displayContentOfLevel(list_levelid[tag].asInt());
    
//    auto tag = (int)static_cast<Button *>(sender)->getTag()-100;
//    CCLOG("level tag = %d", tag);
//    if (tag == index_selected) {
//        return;
//    }
//    listview_level->removeItem(index_selected);
//    listview_level->insertCustomItem(levelButtonWith(100+index_selected, list_levelid[index_selected].asInt()), index_selected);
//    
//    listview_level->removeItem(tag);
//    listview_level->insertCustomItem(levelButtonWith(100+tag, list_levelid[tag].asInt(), true), tag);
//    
//    index_selected = tag;
//    if (_isPortrait) {
//        listview_level->jumpToItem(index_selected, Vec2(0.5, 0), Vec2::ANCHOR_BOTTOM_LEFT);
//    }else listview_level->jumpToItem(index_selected, Vec2(0, 0.5), Vec2::ANCHOR_TOP_LEFT);
//
////    if (((int)list_levelid.size() - index_selected +1)*124 > listview_level->getContentSize().width) {
////        listview_level->jumpToItem(index_selected, Vec2(0, 0.5), Vec2::ANCHOR_TOP_LEFT);
////    }else listview_level->jumpToItem(index_selected, Vec2(1, 0.5), Vec2::ANCHOR_TOP_RIGHT);
//
//    CCLOG("vecX = %f", listview_level->getItem(index_selected)->getContentSize().width*(index_selected+1));
}

void StoryReadingLevel::onShowHidePopUp(bool isShow){
    if (isRunning) return;
    isRunning = true;
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/StoryReadingLevel.csb"));
    runani->setTag(1011);
    this->runAction(runani);
    if (isShow) {
        runani->play("show", false);
    }else runani->play("hide", false);
    runani->setLastFrameCallFunc([=]{
        isRunning = false;
        if (!isShow) {
            if (isNextClicked) {
                this->getChildByName("layout_welcome")->setVisible(false);
                Node *layout_bg;
                if (_isPortrait) {
                    layout_bg = this->getChildByName("layout_doc");
                }else{
                    layout_bg = this->getChildByName("layout_ngang");
                }
                layout_bg->setVisible(true);
        
                this->onShowHidePopUp(true);
            }else{
                this->removeFromParent();
            }
        }else{
            if (isNextClicked) {
                isNextClicked = false;
                this->displayReadingLevel();
            }
        }
    });
}

#pragma mark - BUTTON
void StoryReadingLevel::onClose(cocos2d::Ref *sender){
    onShowHidePopUp(false);
}

void StoryReadingLevel::onNext(cocos2d::Ref *sender){
    isNextClicked = true;
    MJDEFAULT->setBoolForKey(key_readinglevel_next, true);
    onShowHidePopUp(false);
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryReadingLevel::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryReadingLevel, onClose),
        CLICK_MAP(StoryReadingLevel, onNext),

    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}

//1. Yêu và xa và quên tựa những xa xôi ngày ấy
//Kỉ niệm nơi đây thấy buồn biết mấy
//Rồi mai tỉnh giấc liệu em
//Còn cất anh trong tiềm thức
//Còn bật khóc vì những ngày như mơ giờ tan vỡ.
//
//[ĐK:]
//Vì đã có lúc trái tim anh từng yếu đuối
//Tự cho phép mình không phải quên
//Tự xem như mình vẫn gần bên
//Và nếu lúc ấy thấy em vẫn còn nơi đây
//Anh sẽ đến và ôm chặt em
//Để thương nhớ chìm trong màn đêm không màu.
//
//2. Ngăn làm sao nỗi xót xa rơi trên mắt người
//Quên làm sao tiếng ca chưa ngân hết lời
//Đông còn chưa đến cớ sao môi anh vẫn run
//Anh còn chưa đến mà sao em đã đi.
//
//[ĐK]
//
//[Bridge:]
//Biến nỗi nhớ trong anh thành tiếng hát
//Xua tan đi cô đơn bấy lâu của ngày không em
//Dù vẫn biết có cố gắng cũng chỉ là hư vô thế thôi
//Anh vẫn không bao giờ tin rằng anh đã mất em.
//
//[ĐK]
//
//* Anh vẫn yêu em, anh vẫn yêu em
//Và nếu lúc ấy thấy em vẫn còn nơi đây
//Anh sẽ đến và ôm chặt em
//Để thương nhớ chìm trong màn đêm không màu
