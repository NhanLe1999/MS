//
//  MSFeaturedStories.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/8/17.
//

#include "MSFeaturedStories.h"
#include "StoryDataModel.h"
#include "RemoteImageView.h"
#include "StoryInfoCover.h"

USING_NS_CC;
INITIALIZE_READER(MSFeaturedStories);

#define FEATURED_TIME_INTERVAL 5.f

std::once_flag ms_featured_stories_reader;
MSFeaturedStories * MSFeaturedStories::createView() {
    std::call_once(ms_featured_stories_reader, [] {
        REGISTER_CSB_READER(MSFeaturedStories);
    });
    
     auto csb_name = mj::helper::getCSBName("csb/new_ui/Featured.csb");
    auto view = reinterpret_cast<MSFeaturedStories *>(CSLoader::createNode(csb_name));
    if (view) {
        view->didLoadFromCSB();
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return nullptr;
}

void MSFeaturedStories::didLoadFromCSB() {
    auto featured = StoryDataModel::getInstance()->getFeaturedStoriesByLanguage(1);
    if (featured.empty()) {
        return;
    }
    
    _pages = utils::findChild<ui::Layout *>(this, "page");
    _bt_forward = utils::findChild<ui::Button *>(this, "button_forward");
    _bt_backward = utils::findChild<ui::Button *>(this, "button_backward");
    
    _indicators = utils::findChild<ui::ListView *>(this, "indicator");
    auto dot_model = utils::findChild<ui::CheckBox *>(this, "dot");
    
    for (auto f : featured) {
        auto model = reinterpret_cast<ui::CheckBox *>(dot_model->clone());
        model->setSelected(false);
        _indicators->pushBackCustomItem(model);
    }
    _indicators->setContentSize(Size((int)featured.size() * (dot_model->getContentSize().width + _indicators->getItemsMargin()) - _indicators->getItemsMargin(), dot_model->getContentSize().height));
    
    auto total_featured = (int)featured.size();
    if (total_featured == 1) {
        total_featured = 5;
    } else if (total_featured < 5) {
        total_featured *= 3;
    }
    
    for (auto i = 0; i < total_featured; ++i) {
        auto idx = i % (int) featured.size();
        auto story = featured[idx];
        auto image = RemoteImageView::createImage(story.second, [](RemoteImageView * img, bool success) {
            if (success) {
                img->setContentSize(Size(700, 200));
                img->setOpacity(0);
                img->runAction(FadeIn::create(0.25));
                img->removeChildByName("featured_title");
            }
        });
        image->setContentSize(Size(700, 200));
        image->setTag(story.first);
        image->setTouchEnabled(true);
        image->setPropagateTouchEvents(true);
        image->setSwallowTouches(false);
        image->addClickEventListener([](Ref * sender) {
            return;
        });
        image->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
        image->setPosition(Point(i * 700, 0));
        
        auto info = StoryDataModel::getInstance()->getStoryById(featured[idx].first);
        
        auto title = ui::Text::create(info.name, "fonts/Montserrat-SemiBold.ttf", 20);
        title->setPosition(Point(350, 100));
        title->setTextColor(Color4B::BLACK);
        title->setName("featured_title");
        image->addChild(title, -1);
        
        _pages->addChild(image);
        _nodes.push_back(image);
    }
    
    _current_idx = total_featured / 2;
    _pages->setPosition(Point(- _current_idx * 700, 0));
    auto indicator = reinterpret_cast<ui::CheckBox *>(_indicators->getItem(_current_idx % (int)featured.size()));
    indicator->setSelected(true);

    this->schedule(schedule_selector(MSFeaturedStories::updateFeatured), FEATURED_TIME_INTERVAL);
    
    auto content = utils::findChild<ui::Widget *>(this, "content");
    content->addTouchEventListener([=](Ref * sender, ui::Widget::TouchEventType type) {
        auto widget = reinterpret_cast<ui::Widget *>(sender);
        switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
                this->unschedule(schedule_selector(MSFeaturedStories::updateFeatured));
                _bt_backward->setEnabled(false);
                _bt_forward->setEnabled(false);
                break;
                
            case ui::Widget::TouchEventType::MOVED: {
                auto begin_position = widget->getTouchBeganPosition();
                auto moved_position = widget->getTouchMovePosition();
                _pages->setPosition(Point(_current_idx * (-700), 0) + Point(moved_position.x - begin_position.x, 0));
                break;
            }
                
            case ui::Widget::TouchEventType::ENDED:
            case ui::Widget::TouchEventType::CANCELED: {
                this->schedule(schedule_selector(MSFeaturedStories::updateFeatured), FEATURED_TIME_INTERVAL);
                
                auto begin_position = widget->getTouchBeganPosition();
                auto moved_position = widget->getTouchEndPosition();
                
                auto dx = moved_position.x - begin_position.x;
                if (fabs(dx) < 200) {
                    _pages->runAction(Sequence::create(EaseQuarticActionOut::create(MoveTo::create(0.5, Vec2(_current_idx * (-700), 0))), CallFunc::create([=] {
                        _bt_backward->setEnabled(true);
                        _bt_forward->setEnabled(true);
                    }), NULL));
                    
                    if (fabs(dx) < 5) {
                        auto node = _nodes.at((int)_nodes.size() / 2);
                        CCLOG("Clicked feature %d", node->getTag());
            
                        auto info = StoryDataModel::getInstance()->getStoryById(node->getTag());
                        auto storyinfocover = static_cast<StoryInfoCover *>(CSLoader::createNode("csb/StoryInfoCover.csb"));
                        StoryInfoObj obj;
                        obj.name = info.name;
                        obj.mostpopular = info.total_words;
                        obj.readinglevel = info.levels[0];
                        obj.languageid = info.lang_id;
                        obj.storyid = info.story_id;
                        obj.newest = info.unique_id;
                        obj.total_page = info.total_page;
                        obj.word_count = info.total_words;
                        obj.lexile = info.lexile;
                        obj._id = info.unique_id;
                        obj._storyid = info.story_id;
                        obj._version = info.version;
                        obj.downloaded = false;
                        obj.thumbnail = info.thumbnail_url;
                        obj.written = info.author;
                        obj.design = info.designer;
                        obj.image = info.thumbnail_url;
                        storyinfocover->initWithData(obj, false);
            
						MJDEFAULT->setStringForKey("STORY_START_FROM", "feature");

                        Director::getInstance()->getRunningScene()->addChild(storyinfocover, 1000);
                    }
                } else {
                    if (dx > 0) {
                        this->moveBackward();
                    } else {
                        this->moveForward();
                    }
                }
                
                break;
            }
        }
    });
}

void MSFeaturedStories::updateFeatured(float dt) {
    moveForward();
}

void MSFeaturedStories::moveForward() {
    if (_nodes.empty()) {
        return;
    }
    auto front = _nodes.front();
    auto back = _nodes.back();
    front->setPosition(back->getPosition() + Vec2(700, 0));
    _nodes.erase(_nodes.begin());
    _nodes.push_back(front);
    
    _current_idx++;
    
    auto items = _indicators->getItems();
    for (auto i : items) {
        auto cb = reinterpret_cast<ui::CheckBox *>(i);
        cb->setSelected(false);
    }
    
    auto temp_idx = _current_idx + (int) items.size() * 100000;
    
    auto indicator = reinterpret_cast<ui::CheckBox *>(_indicators->getItem(temp_idx % (int)items.size()));
    indicator->setSelected(true);
    
    _bt_backward->setEnabled(false);
    _bt_forward->setEnabled(false);
    _pages->runAction(Sequence::create(EaseQuarticActionOut::create(MoveTo::create(0.5, Vec2(_current_idx * (-700), 0))), CallFunc::create([=] {
        _bt_backward->setEnabled(true);
        _bt_forward->setEnabled(true);
    }), NULL));
}

void MSFeaturedStories::moveBackward() {
    if (_nodes.empty()) {
        return;
    }
    auto front = _nodes.front();
    auto back = _nodes.back();
    back->setPosition(front->getPosition() + Vec2(-700, 0));
    _nodes.erase(_nodes.begin() + _nodes.size() - 1);
    _nodes.insert(_nodes.begin(), back);
    
    _current_idx--;
    
    CCLOG("current id: %d", _current_idx);
    
    auto items = _indicators->getItems();
    for (auto i : items) {
        auto cb = reinterpret_cast<ui::CheckBox *>(i);
        cb->setSelected(false);
    }

    auto temp_idx = _current_idx + (int) items.size() * 100000;
    
    auto indicator = reinterpret_cast<ui::CheckBox *>(_indicators->getItem(temp_idx % (int)items.size()));
    indicator->setSelected(true);
    
    _bt_backward->setEnabled(false);
    _bt_forward->setEnabled(false);
    _pages->runAction(Sequence::create(EaseQuarticActionOut::create(MoveTo::create(0.5, Vec2(_current_idx * (-700), 0))), CallFunc::create([=] {
        _bt_backward->setEnabled(true);
        _bt_forward->setEnabled(true);
    }), NULL));
}

void MSFeaturedStories::setContentSize(const Size & size) {
    ui::Layout::setContentSize(size);
    ui::Helper::doLayout(this);
    
    auto left_panel = utils::findChild(this, "panel_left");
    auto right_panel = utils::findChild(this, "panel_right");
    auto center = utils::findChild(this, "content");
    
    if (center && left_panel && right_panel) {
        auto bb = center->getBoundingBox();
        left_panel->setContentSize(Size(bb.getMinX(), left_panel->getContentSize().height));
        right_panel->setContentSize(Size(size.width - bb.getMaxX(), right_panel->getContentSize().height));
    }
}

ui::Widget::ccWidgetClickCallback MSFeaturedStories::onLocateClickCallback(const std::string &callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSFeaturedStories, onForward),
        CLICK_MAP(MSFeaturedStories, onBackward)
    };
    
    if (function_map.find(callback_name) != function_map.end()) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSFeaturedStories::onForward(cocos2d::Ref *sender) {
    this->unschedule(schedule_selector(MSFeaturedStories::updateFeatured));
    this->schedule(schedule_selector(MSFeaturedStories::updateFeatured), FEATURED_TIME_INTERVAL);
    moveForward();
}

void MSFeaturedStories::onBackward(cocos2d::Ref *sender) {
    this->unschedule(schedule_selector(MSFeaturedStories::updateFeatured));
    this->schedule(schedule_selector(MSFeaturedStories::updateFeatured), FEATURED_TIME_INTERVAL);
    moveBackward();
}
