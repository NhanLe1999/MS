//
//  MSActivityPreview.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/14/17.
//

#include "MSActivityPreview.h"
#include "StoryDataModel.h"
#include "RemoteImageView.h"
#include "StoryLanguageManager.h"
#include "MJDefinitions.h"
#include "MSLessonManager.h"
USING_NS_CC;
INITIALIZE_READER(MSActivityPreview);

std::once_flag ms_activity_preview_reader;
MSActivityPreview * MSActivityPreview::createView(mj::model::ActInfo info, bool is_first, bool is_last) {
    std::call_once(ms_activity_preview_reader, [] {
        REGISTER_CSB_READER(MSActivityPreview);
    });
    
    auto csb = info.type == mj::model::ActInfo::Type::STORY ? "csb/new_ui/lesson/ActivityPreview_Story.csb" : "csb/new_ui/lesson/ActivityPreview_Game.csb";
    auto node = reinterpret_cast<MSActivityPreview *>(CSLoader::createNode(csb));
    if (node) {
        node->didLoadFromCSB(info, is_first, is_last);
        return node;
    }
    
    CC_SAFE_DELETE(node);
    return nullptr;
}

#include "PhonicConfig.h"
#include "GameList.h"
void MSActivityPreview::didLoadFromCSB(mj::model::ActInfo info, bool is_first, bool is_last) {
    _info = info;
    
    auto extracted_name = mj::helper::extractActivityName(_info.name);
    
    auto lesson = ms::LessonManager::getInstance().getLessonById(info.lesson_id);
    
    utils::findChild<ui::Text *>(this, "name")->setString(extracted_name);
    utils::findChild<ui::Text *>(this, "content")->setString(_info.content);
    
    utils::findChild<ui::Text *>(this, "placeholder")->setString(extracted_name);
    
    changeTextDisplayLang();
    auto thumb_placeholder = utils::findChild(this, "preview");
    utils::findChild(this, "mask")->setLocalZOrder(1);
    utils::findChild(this, "mask")->setColor(Color3B(244, 245, 246));
    if (_info.type == mj::model::ActInfo::STORY) {
        auto story = StoryDataModel::getInstance()->getStoryById(_info.sub_id);
        utils::findChild<ui::Text *>(this, "placeholder")->setString(story.name);
        auto remoteview = RemoteImageView::createImage(story.thumbnail_url, [=](RemoteImageView * img, bool is_success) {
            if (is_success) {
                utils::findChild(this, "image")->setVisible(false);
            }
            img ? img->setContentSize(utils::findChild(this, "image")->getContentSize()) : (void) nullptr;
        }, RemoteImageView::Type::STORIES);
        
        remoteview->setPosition(thumb_placeholder->getContentSize() / 2);
        //remoteview->setName("story_thumb");
        remoteview->setContentSize(utils::findChild(this, "image")->getContentSize());
        auto img = remoteview->getChildByName<ui::ImageView *>("r_image");
        if (_info.state == mj::model::ActInfo::State::LOCKED) {
            mj::helper::grayscaleImageView(img);
            remoteview->setOpacity(100);
        } else {
            mj::helper::grayscaleImageView(img, false);
        }
        thumb_placeholder->addChild(remoteview);
    } else {
        auto game = StoryDataModel::getInstance()->getGameById(_info.sub_id);
        auto remoteview = RemoteImageView::createImage(game.thumbnail_url, [=](RemoteImageView * img, bool is_success) {
            if (is_success) {
                utils::findChild(this, "image")->setVisible(false);
            }
            img ? img->setContentSize(utils::findChild(this, "image")->getContentSize()) : (void) nullptr;
        }, RemoteImageView::Type::OTHER, true);
        
        remoteview->setPosition(thumb_placeholder->getContentSize() / 2);
        //remoteview->setName("game_thumb");
        remoteview->setContentSize(utils::findChild(this, "image")->getContentSize());
        auto img = remoteview->getChildByName<ui::ImageView *>("r_image");
        if (_info.state == mj::model::ActInfo::State::LOCKED) {
            mj::helper::grayscaleImageView(img);
            remoteview->setOpacity(100);
        } else {
            mj::helper::grayscaleImageView(img, false);
        }
        thumb_placeholder->addChild(remoteview);
    }
    
    auto draw = DrawNode::create();
    this->addChild(draw);

    auto dot_locked = utils::findChild(this, "dot_locked");
    auto icon_locked = utils::findChild(this, "status_locked");
    
    auto dot_learned = utils::findChild(this, "dot_learned");
    auto icon_learned = utils::findChild(this, "status_learned");
    
    auto dot_ready = utils::findChild(this, "dot_ready");
    auto icon_ready = utils::findChild(this, "status_ready");
    
    dot_ready->setLocalZOrder(1);
    icon_ready->setLocalZOrder(1);
    
    dot_learned->setLocalZOrder(1);
    icon_learned->setLocalZOrder(1);
    
    dot_locked->setLocalZOrder(1);
    icon_locked->setLocalZOrder(1);
    
    Color3B line_color, line_left_color, line_right_color;
    
    dot_ready->setVisible(_info.state == mj::model::ActInfo::READY);
    icon_ready->setVisible(_info.state == mj::model::ActInfo::READY);
    
    dot_locked->setVisible(_info.state == mj::model::ActInfo::LOCKED);
    icon_locked->setVisible(_info.state == mj::model::ActInfo::LOCKED);
    
    dot_learned->setVisible(_info.state == mj::model::ActInfo::LEARNED);
    icon_learned->setVisible(_info.state == mj::model::ActInfo::LEARNED);
    
    switch (_info.state) {
        case mj::model::ActInfo::LOCKED:
            line_color = Color3B(197, 197, 197);
            line_left_color = Color3B(197, 197, 197);
            line_right_color = Color3B(197, 197, 197);
            utils::findChild(this, "title")->setOpacity(100);
//            utils::findChild(this, "button_play")->removeFromParent();
            break;
        case mj::model::ActInfo::READY:
            line_color = Color3B(0, 178, 234);
            line_left_color = Color3B(89, 171, 43);
            line_right_color = Color3B(197, 197, 197);
            break;
        case mj::model::ActInfo::LEARNED:
            line_color = Color3B(89, 171, 43);
            line_left_color = Color3B(89, 171, 43);
            line_right_color = Color3B(89, 171, 43);
            break;
    }
    
    draw->drawLine(dot_ready->getPosition(), icon_ready->getPosition(), Color4F(line_color));
    if (!is_last) {
        draw->drawLine(icon_ready->getPosition(), Point(this->getContentSize().width, icon_ready->getPositionY()), Color4F(line_right_color));
    }
    
    if (!is_first) {
        draw->drawLine(icon_ready->getPosition(), Point(0, icon_ready->getPositionY()), Color4F(line_left_color));
    }
    draw->setLineWidth(4);
}

void MSActivityPreview::onEnter() {
    ui::Layout::onEnter();
    auto change_lang_listener = EventListenerCustom::create(key_change_displaylang, [this](EventCustom *) {
        this->changeTextDisplayLang();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(change_lang_listener, this);
}

void MSActivityPreview::changeTextDisplayLang()
{
    auto idx = ms::LessonManager::getInstance().getActivityIndexOfLesson(_info);
    _info.order = idx;

    utils::findChild<ui::Text *>(this, "activity")->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawad.ttf":"fonts/Montserrat-Regular.ttf");
    utils::findChild<ui::Text *>(this, "activity")->setString(StringUtils::format("%s %d:", LANGUAGE_MANAGER->getTextByKey(LANGUAGE_MANAGER->getDisplayLangId(), "lesson.actipreview.acti").c_str(), idx + 1));
}

ui::Widget::ccWidgetClickCallback MSActivityPreview::onLocateClickCallback(const std::string &name) {
    if (name == "onSelectActivity") {
        return CC_CALLBACK_1(MSActivityPreview::onSelectActivity, this);
    }
    
    return nullptr;
}

#include "StoryConfigManager.h"

void MSActivityPreview::onSelectActivity(cocos2d::Ref *sender) {
    mj::helper::playButtonFX();
    if (CONFIG_MANAGER->isLearnAllLessons()) {
        EventCustom e("play_activity");
        e.setUserData(&_info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
        return;
    }
    if (_info.state == mj::model::ActInfo::LOCKED) {
        EventCustom e("play_activity_locked");
        e.setUserData(&_info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    } else {
        EventCustom e("play_activity");
        e.setUserData(&_info);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
    }
}

