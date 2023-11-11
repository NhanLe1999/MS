//
//  StoryLevelInfo.cpp
//  MJStory
//
//  Created by HS9 on 11/04/15.
//
//

#include "StoryLevelInfo.h"
#include "HSLib.h"
#include "ui/UIHelper.h"
#include "NativeBridge.h"
#include "StoryDataModel.h"
#define text_marin 50
INITIALIZE_READER_INSTANCE(StoryLevelInfoReader);

USING_NS_CC;
using namespace ui;

StoryLevelInfo * StoryLevelInfo::createPage() {
    auto p = static_cast<StoryLevelInfo *>(CSLoader::createNodeWithVisibleSize("csb/StoryLevelInfo.csb"));
    if (p) {
        p->didLoadFromCSB();
    }
    return p;
}

void StoryLevelInfo::didLoadFromCSB() {
    
    
}
#pragma mark - onEnter
void StoryLevelInfo::onEnter() {
    Layout::onEnter();
    auto lang_id = MJDEFAULT->getIntegerForKey(key_lang_display, 1);
    list_level_des = StoryDataModel::getInstance()->getAllLevelInfo(lang_id);
    list_levelid = JsonDump::getInstance()->dumpValueVectorFromString(MJDEFAULT->getStringForKey(StringUtils::format("%s%d", key_level_listlevellang, lang_id)));

    if (_isPortrait) {
        std::reverse(list_level_des.begin(), list_level_des.end());
        std::reverse(list_levelid.begin(), list_levelid.end());
    }
    
    Node *layout_bg;
    if (_isPortrait) {
        layout_bg = this->getChildByName("layout_doc");
    }else{
        layout_bg = this->getChildByName("layout_ngang");
    }
    layout_bg->setVisible(true);
    auto bg = layout_bg->getChildByName("bg");
    auto layout_story = bg->getChildByName("layout_story");
    
    lexile_lb = layout_story->getChildByName<Text *>("lexile_lb");
    word_lb = layout_story->getChildByName<Text *>("word_lb");
    page_lb = layout_story->getChildByName<Text *>("page_lb");
    lexile_value_lb = layout_story->getChildByName<Text *>("lexile_value_lb");
    word_value_lb = layout_story->getChildByName<Text *>("word_value_lb");
    page_value_lb = layout_story->getChildByName<Text *>("page_value_lb");
    page_value_lb->setString(StringUtils::toString(_storyInfo.total_page));
    
    lexile_lb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "lexile_score"));
    word_lb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "word_count"));
    page_lb->setString(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(classes_name, "page_count"));
    
    word_value_lb->setString(StringUtils::toString(_storyInfo.word_count));
    if (_storyInfo.lexile == "") {
        lexile_value_lb->setString("0L - 100L");
    }else lexile_value_lb->setString(_storyInfo.lexile);
    
    
    auto title = layout_story->getChildByName("title");
    title->setVisible(false);
    auto namelb = Label::createWithTTF(_storyInfo.name, "fonts/monkeystories_medium.ttf", 28);
    layout_story->addChild(namelb);
    namelb->setPosition(title->getPosition());
    namelb->setTextColor(Color4B::BLACK);
    namelb->setWidth(layout_story->getContentSize().width*0.9);
    namelb->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    if (namelb->getStringNumLines() > 1) {
        namelb->setPositionY(namelb->getPositionY()+8);
    }
    if (namelb->getStringNumLines() > 2) {
        namelb->setTTFConfig(TTFConfig("fonts/monkeystories_medium.ttf",13));
    }
    
    thumb = bg->getChildByName<ImageView *>("thumb");
    auto path_image = _storyInfo.image;
    if (path_image != "") {
        auto pos = path_image.find_last_of('/');
        path_image = path_image.substr(pos);
        path_image = FileUtils::getInstance()->getWritablePath() + image_cache_path + path_image;
    }else {
        path_image = string("mjstory/home/mjsh_stories_thumb_1.png");
    }
    thumb->loadTexture(path_image);
    
    listview_level = layout_bg->getChildByName<ListView *>("listview_level");
    listview_level->setClippingEnabled(true);
    listview_level->setScrollBarEnabled(false);
    //listview_level->setMagneticType(ListView::MagneticType::BOTH_END);
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
    
    
//    if (!MJDEFAULT->getBoolForKey(key_display_levelguide)) {
//        MJDEFAULT->setBoolForKey(key_display_levelguide, true);
//    }
}

void StoryLevelInfo::initWithData(StoryInfoObj storyInfo, bool isPortrait){
    _storyInfo = storyInfo;
    _isPortrait = isPortrait;
}

void StoryLevelInfo::onEnterTransitionDidFinish(){
    Layout::onEnterTransitionDidFinish();
}
#pragma mark - onExit
void StoryLevelInfo::onExitTransitionDidStart(){
    Layout::onExitTransitionDidStart();
}

#pragma mark - LOGIC_FUNTION
void StoryLevelInfo::displayListLevel(){
    for (int i = 0; i < (int) list_levelid.size(); i++) {
        Layout *bt;
        CCLOG("id = %d", list_levelid[i].asInt());
        if (list_levelid[i].asInt() == _storyInfo.readinglevel) {
            index_selected = i;
             bt = levelButtonWith(100+i, list_levelid[i].asInt(), true);
        }else {
            bt = levelButtonWith(100+i, list_levelid[i].asInt());
        }
        listview_level->addChild(bt);
    }
    if (_isPortrait) {
        listview_level->jumpToBottom();
    }else listview_level->jumpToLeft();

    if (_isPortrait) {
        listview_level->jumpToItem(index_selected, Vec2(0.5, 0), Vec2::ANCHOR_BOTTOM_LEFT);
    }else listview_level->jumpToItem(index_selected, Vec2(0, 0.5), Vec2::ANCHOR_TOP_LEFT);
    
    displayContentOfLevel(list_levelid[index_selected].asInt());
}
void StoryLevelInfo::displayContentOfLevel(int level_id){
    auto map_level_info = list_level_des[index_selected].asValueMap();
//    content_level_lb->setString("Sáng nay mình em trước gương chợt nhớ một người em rất thương. Người đã khiến cuộc sống em khác đi rất nhiều. Và nếu như tình anh vẫn thế thì Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm, và yêu người ấy thật nhiều! Lời chia tay buồn đến mấy cũng không thể nào làm cho anh gục ngã đến mức tuyệt vọng, chỉ là vết thương sâu một chút thôi anh à. Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm, và yêu người ấy thật nhiều! Lời chia tay buồn đến mấy cũng không thể nào làm cho anh gục ngã đến mức tuyệt vọng, chỉ là vết thương sâu một chút thôi anh à. Ngày mà anh tìm đến em tin anh thật lòng và yêu anh bằng những cảm xúc tự nguyện. Vì em quá yêu anh mù quáng đến sai lầm");
    content_level_lb->setString(map_level_info["des"].asString());
    if (_isPortrait) {
        auto text_height = content_level_lb->getContentSize().height;
        if (text_height > scroll_content->getContentSize().width) {
            scroll_content->setInnerContainerSize(cocos2d::Size(content_level_lb->getContentSize().height+text_marin/2, scroll_content->getContentSize().height));
        }else{
            scroll_content->setInnerContainerSize(scroll_content->getContentSize());
        }
        content_level_lb->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
        content_level_lb->setPosition(Vec2(text_marin/2, scroll_content->getContentSize().height - text_marin));

    }else{
        auto text_height = content_level_lb->getContentSize().height;
        if (text_height > scroll_content->getContentSize().height) {
            scroll_content->setInnerContainerSize(cocos2d::Size(scroll_content->getContentSize().width, content_level_lb->getContentSize().height+text_marin/2));
        }else{
            scroll_content->setInnerContainerSize(scroll_content->getContentSize());
            content_level_lb->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
            content_level_lb->setPosition(Vec2(text_marin, scroll_content->getContentSize().height - text_marin/2));
        }
    }
}

Layout* StoryLevelInfo::levelButtonWith(int tag, int level_id, bool isSelect){
    auto layout = Layout::create();
    auto bt = Button::create("mjstory/popup/levelinfostory_item.png", "mjstory/popup/levelinfostory_item.png", "mjstory/popup/levelinfostory_itemchoose.png");

    if (_isPortrait) {
        layout->setContentSize(cocos2d::Size(bt->getContentSize().height, bt->getContentSize().width));
        bt->setRotation(-90);
    }else layout->setContentSize(bt->getContentSize());
    layout->addChild(bt);
    bt->setAnchorPoint(Vec2(0, 0));
    bt->setName("button");
    auto text = Text::create(LANGUAGE_MANAGER->getTextObjectWithKeyOfClass(general_lang, "level")+ " " + XHSLib->getNameLevelWithID(level_id), "fonts/monkeystories_medium.ttf", 20);
    bt->addChild(text);
    text->setPosition(Vec2(bt->getContentSize().width/2, bt->getContentSize().height/2));
    text->setName("label");
    if (isSelect) {
        if (_isPortrait) {
            bt->setPosition(Vec2(12, 0));
            bt->setAnchorPoint(Vec2(0, 1));
        }else {
            bt->setPosition(Vec2(0, -12));
        }
        text->setPosition(Vec2(bt->getContentSize().width/2, bt->getContentSize().height/2-5));
        text->setColor(Color3B(46,49,144));
        bt->setEnabled(false);
        bt->setBright(false);
    }else {
        if (_isPortrait) {
            bt->setPosition(Vec2(17, 0));
            bt->setAnchorPoint(Vec2(0, 1));
        }else {
            bt->setPosition(Vec2(0, -17));
        }
        text->setColor(Color3B(2,155,214));
    }
    text->setScaleX(0.95);
    bt->addClickEventListener(CC_CALLBACK_1(StoryLevelInfo::onChooseLevel, this));
    bt->setTag(tag);
    return layout;
}

void StoryLevelInfo::onChooseLevel(cocos2d::Ref *sender){
    auto tag = (int)static_cast<Button *>(sender)->getTag()-100;
    for (auto item : listview_level->getItems()) {
        auto layout = dynamic_cast<cocos2d::ui::Layout *>(item);
        if (!layout) return;
        auto button = layout->getChildByName<Button *>("button");
        if (!button) return;
        if ((button->getTag()-100) == tag) {
            auto text = button->getChildByName<cocos2d::ui::Text *>("label");
            text->setColor(Color3B(46,49,144));
            
            button->setEnabled(false);
            button->setBright(false);
            if (_isPortrait) {
                button->setPosition(Vec2(12, 0));
                button->setAnchorPoint(Vec2(0, 1));
            }else {
                button->setPosition(Vec2(0, -12));
            }
            text->setPosition(Vec2(button->getContentSize().width/2, button->getContentSize().height/2-5));
        }else {
            auto text = button->getChildByName<cocos2d::ui::Text *>("label");
            text->setColor(Color3B(2,155,214));
            
            button->setEnabled(true);
            button->setBright(true);
            if (_isPortrait) {
                button->setPosition(Vec2(17, 0));
                button->setAnchorPoint(Vec2(0, 1));
            }else {
                button->setPosition(Vec2(0, -17));
            }
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
//    displayContentOfLevel(list_levelid[index_selected].asInt());
}

void StoryLevelInfo::onShowHidePopUp(bool isShow){
    if (isRunning) return;
    isRunning = true;
    auto runani = CSLoader::createTimeline(StringUtils::format("csb/StoryLevelInfo.csb"));
    runani->setTag(1011);
    this->runAction(runani);
    if (isShow) {
        runani->play("show", false);
    }else runani->play("hide", false);
    runani->setLastFrameCallFunc([=]{
        isRunning = false;
        if (!isShow) {
            this->removeFromParent();
        }else{
        }
    });
}

#pragma mark - BUTTON
void StoryLevelInfo::onClose(cocos2d::Ref *sender){
    onShowHidePopUp(false);
}

#pragma mark - Callback Functions
Widget::ccWidgetClickCallback StoryLevelInfo::onLocateClickCallback(const std::string &callback_name) {
    std::map<std::string, Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(StoryLevelInfo, onClose),

    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    return nullptr;
}


