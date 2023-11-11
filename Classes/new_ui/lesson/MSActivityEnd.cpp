
//
//  MSActivityEnd.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/22/17.
//

#include "MSActivityEnd.h"
#include "MSLessonManager.h"
#include "StoryDataModel.h"
#include "RemoteImageView.h"

#include "MSPreActivity.h"
#include "AudioEngine.h"
#include "MSActivityWrapper.h"
#include "MJDefinitions.h"
#include "StoryConfigManager.h"
#include "PhonicConfig.h"
#include "MJPlatformConfig.h"
#include "HelperManager.h"

#define key_event_play_activity_name "key.event.play.activity.name"
#define COUTN_CONTIUE 15

USING_NS_CC;
INITIALIZE_READER(MSActivityEnd);

static bool _isExist = false;

std::once_flag ms_activity_end_reader;
MSActivityEnd * MSActivityEnd::createPopup(mj::model::ActInfo activity, bool animate_next) {
    std::call_once(ms_activity_end_reader, [] {
        REGISTER_CSB_READER(MSActivityEnd);
    });
    
    auto popup = reinterpret_cast<MSActivityEnd *>(CSLoader::createNodeWithVisibleSize("csb/new_ui/lesson/ActivityEnd.csb"));
    if (popup) {
        popup->didLoadFromCSB(activity, animate_next);
        return popup;
    }
    
    CC_SAFE_DELETE(popup);
    return nullptr;
}

#include "MsNewDownloadFile.h"

void MSActivityEnd::didLoadFromCSB(mj::model::ActInfo activity, bool animate_next) {

    if(!ms::LessonManager::getInstance().isActivityReady(activity));
    {
        ms::LessonManager::getInstance().prepareActivity(activity, nullptr, false, true);
    }

    _isExist = false;
   // HelperManager::GetInstance()->addSidebarBlack();
    setCallbackActivityName();
    if (CONFIG_MANAGER->isVietnameseStories()) utils::findChild(this, "button_play")->setVisible(false);
    _info = activity;
    _info.order = ms::LessonManager::getInstance().getActivityIndexOfLesson(_info);
    _is_animate_next = animate_next;
    
    _next_info = ms::LessonManager::getInstance().getNextActivity(activity);
    if (_next_info.activity_id != -1) {
        _next_info.order = ms::LessonManager::getInstance().getActivityIndexOfLesson(_next_info);
    }
    auto lesson = ms::LessonManager::getInstance().getLessonById(activity.lesson_id);
//    utils::findChild<ui::Text *>(this, "lesson_name")->setString(lesson.name);
    if (CONFIG_MANAGER->isVietnameseStories()){
        utils::findChild<Text*>(this, "Text_1")->setString("Bắt đầu");
    }
    utils::findChild<ui::Text *>(this, "activity_order")->setString(StringUtils::format(CONFIG_MANAGER->isVietnameseStories()?"Hoạt động %d":"Activity %d", activity.order + 1));
    
    utils::findChild<ui::Text *>(this, "activity_name")->setString(mj::helper::extractActivityName(activity.name));

    auto activities = StoryDataModel::getInstance()->getActivitiesOfLesson(lesson.lesson_id);
    auto index = ms::LessonManager::getInstance().getActivityIndexOfLesson(activity);
    
    auto checked = utils::findChild<ui::ImageView *>(this, "img_checked");
    auto current = utils::findChild<ui::ImageView *>(this, "img_current");
    auto normal = utils::findChild<ui::ImageView *>(this, "img_normal");
    auto progress = utils::findChild<ui::ImageView *>(this, "progress");
    
    if (activities.size() > 7) {
        auto bar_length = 600 - 35 * activities.size();
        bar_length = bar_length / (activities.size() - 1);
        progress->setContentSize(Size(bar_length, 10));
    }
    
    //218.75 296.25
    
    auto listview = utils::findChild<ui::ListView *>(this, "listview");
    
    std::string url;
    cocos2d::Size thumb_size;
    auto info = utils::findChild<ui::ListView *>(this, "info");
    auto info_next = reinterpret_cast<ui::ListView *>(info->clone());
    std::string game_name = "";
    if (activity.type == mj::model::ActInfo::Type::GAME) {
        auto game_info = StoryDataModel::getInstance()->getGameById(activity.sub_id);
        game_name = game_info.name;
        url = game_info.thumbnail_url;
        thumb_size = Size(255, 200);
    } else {
        MJDEFAULT->setStringForKey(key_read_from, "lesson");
        auto story_info = StoryDataModel::getInstance()->getStoryById(activity.sub_id);
        url = story_info.thumbnail_url;
        thumb_size = Size(145, 200);
    }
    auto remote_thumb = RemoteImageView::createImage(url, [info, thumb_size](RemoteImageView * img, bool success) {
        if (success){
            img->setContentSize(thumb_size);
        }
    }, RemoteImageView::Type::OTHER, activity.type == mj::model::ActInfo::Type::GAME);
    remote_thumb->setContentSize(thumb_size);
    info->insertCustomItem(remote_thumb, 0);
    info->doLayout();
    
    auto bb = remote_thumb->getBoundingBox();
    bb.origin = remote_thumb->getParent()->convertToWorldSpace(bb.origin);
    
    auto bb_order = utils::findChild(info, "activity_order")->getBoundingBox();
    bb_order.origin = utils::findChild(info, "activity_order")->getParent()->convertToWorldSpace(bb_order.origin);
    
    auto bb_name = utils::findChild(info, "activity_name")->getBoundingBox();
    bb_name.origin = utils::findChild(info, "activity_name")->getParent()->convertToWorldSpace(bb_name.origin);
    
    bb = bb.unionWithRect(bb_order);
    bb = bb.unionWithRect(bb_name);
    
    info->setContentSize(bb.size);
    
    utils::findChild<ui::Text *>(info_next, "activity_order")->setString(StringUtils::format(CONFIG_MANAGER->isVietnameseStories()?"Hoạt động %d":"Activity %d", _next_info.order + 1));
    utils::findChild<ui::Text *>(info_next, "activity_name")->setString(mj::helper::extractActivityName(_next_info.name));
    if (_next_info.activity_id != -1) {
        if (_next_info.type == mj::model::ActInfo::Type::GAME) {
            auto game_info = StoryDataModel::getInstance()->getGameById(_next_info.sub_id);
            game_name = game_info.name;
            url = game_info.thumbnail_url;
            thumb_size = Size(255, 200);
        }
        else {
            auto story_info = StoryDataModel::getInstance()->getStoryById(_next_info.sub_id);
            url = story_info.thumbnail_url;
            thumb_size = Size(145, 200);
        }
    }
    remote_thumb = RemoteImageView::createImage(url, [info_next, thumb_size](RemoteImageView * img, bool success) {
        if (success){
            info_next->getItem(0)->setContentSize(thumb_size);
        }
    }, RemoteImageView::Type::OTHER, activity.type == mj::model::ActInfo::Type::GAME);
    remote_thumb->setContentSize(thumb_size);
    info_next->insertCustomItem(remote_thumb, 0);
    
    info->getParent()->addChild(info_next);
    info_next->setOpacity(0);
    info_next->doLayout();
    
    bb = remote_thumb->getBoundingBox();
    bb.origin = remote_thumb->getParent()->convertToWorldSpace(bb.origin);
    
    bb_order = utils::findChild(info_next, "activity_order")->getBoundingBox();
    bb_order.origin = utils::findChild(info_next, "activity_order")->getParent()->convertToWorldSpace(bb_order.origin);
    
    bb_name = utils::findChild(info_next, "activity_name")->getBoundingBox();
    bb_name.origin = utils::findChild(info_next, "activity_name")->getParent()->convertToWorldSpace(bb_name.origin);
    
    bb = bb.unionWithRect(bb_order);
    bb = bb.unionWithRect(bb_name);
    
    info_next->setContentSize(bb.size);
    
    auto w = 0;
    
    for (auto i = 0; i < activities.size(); ++i) {
        auto bar = reinterpret_cast<ui::ImageView *>(progress->clone());
        auto loading = bar->getChildByName<ui::LoadingBar *>("bar");
        
        ui::Widget * item;
        if (i < index) {
            item = checked->clone();
            loading->setPercent(100);
        } else if (i == index) {
            item = current->clone();
            loading->setPercent(0);
        } else {
            item = normal->clone();
            loading->setPercent(0);
        }
        
        if (i >= index) {
            auto label = item->getChildByName<ui::Text *>("order");
            label->setString(StringUtils::toString(i + 1));
            label->enableOutline(Color4B(Color3B(18, 77, 96)));
        }
        
        listview->pushBackCustomItem(item);
        w += item->getContentSize().width + listview->getItemsMargin();
        if (i < activities.size() - 1) {
            listview->pushBackCustomItem(bar);
            w += bar->getContentSize().width + listview->getItemsMargin();
        }
        
        item->setLocalZOrder(1);
        bar->setLocalZOrder(-1);
    }
    
    w -= listview->getItemsMargin();
    listview->setContentSize(Size(w, listview->getContentSize().height));
    listview->setAnchorPoint(Point::ANCHOR_MIDDLE);
    listview->setPosition(Point(listview->getParent()->getContentSize().width / 2, listview->getPositionY()));
    
    auto arrow = utils::findChild(this, "arrow");

    auto last = *listview->getItems().rbegin();
    auto width = listview->getContentSize().width - last->getContentSize().width;
    auto segment_length = width / (activities.size() - 1);
    
    auto from_position = Point(segment_length * index + checked->getContentSize().width / 2, -10);
//    from_position = listview->getItem(index * 2)->getPosition();
//    from_position = listview->getItem(index * 2)->getParent()->convertToWorldSpace(from_position);
    from_position = listview->convertToWorldSpace(from_position);
    from_position = arrow->getParent()->convertToNodeSpace(from_position);
    arrow->setPosition(from_position);
    
    auto read_name_listener = EventListenerCustom::create(key_event_play_activity_name, [this](EventCustom *) {
        if (!this) return;
        this->onContinue(nullptr);
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(read_name_listener, this);
    
    auto close_listener = EventListenerCustom::create(key_event_activity_wrapper_ready, [this](EventCustom *) {
        this->removeFromParent();
    });
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(close_listener, this);
    
    if (activities.size() <= 1) {
        return;
    }
    
    if (!animate_next) {
        utils::findChild(this, "button_play")->setVisible(false);
        info->setPosition(info->getPosition() + Point(0, -30));
        listview->setPosition(listview->getPosition() + Point(0, -30));
        arrow->setPosition(arrow->getPosition() + Point(0, -30));
        this->setLayoutComponentEnabled(false);
        
        readActivityName(_info.name);
        return;
    }
    
    info->runAction(Sequence::create(DelayTime::create(1.5), FadeOut::create(1), NULL));
    info_next->runAction(Sequence::create(DelayTime::create(1.5), FadeIn::create(1), NULL));
    int sizeOfListView = listview->getItems().size();
    if (index * 2 + 1 < sizeOfListView)
    {
        auto bar = listview->getItem(index * 2 + 1)->getChildByName<ui::LoadingBar*>("bar");
        if (bar) {
            bar->runAction(Sequence::create(DelayTime::create(1.5), EaseQuinticActionOut::create(ActionFloat::create(1, 0, 100, [bar](float v) {
                bar->setPercent(v);
                })), nullptr));
        }
    }
    
    auto from = listview->getItem(index * 2);
    from->setCascadeOpacityEnabled(false);
    from->runAction(FadeOut::create(1));
    
    auto checked_clone = checked->clone();
    checked_clone->setAnchorPoint(Point::ANCHOR_MIDDLE);
    checked_clone->setPosition(from->getContentSize() / 2);
    checked_clone->setOpacity(0);
    from->addChild(checked_clone);
    checked_clone->runAction(Sequence::create(DelayTime::create(0.5), FadeIn::create(.5), nullptr));
    
    if (index * 2 + 1 < sizeOfListView)
    {
        if (auto target = listview->getItem((index + 1) * 2)) {
            auto current_clone = current->clone();
            current_clone->setAnchorPoint(Point::ANCHOR_MIDDLE);
            current_clone->setPosition(target->getContentSize() / 2);
            current_clone->setOpacity(0);
            target->addChild(current_clone);
            current_clone->runAction(Sequence::create(DelayTime::create(2), FadeIn::create(.5), nullptr));
            auto label = current_clone->getChildByName<ui::Text*>("order");
            label->setString(StringUtils::toString(index + 2));
            label->enableOutline(Color4B(Color3B(18, 77, 96)));

            auto target_position = Point(segment_length * (index + 1) + current->getContentSize().width / 2 + 1, -10);
            target_position = listview->convertToWorldSpace(target_position);
            target_position = arrow->getParent()->convertToNodeSpace(target_position);

            arrow->runAction(Sequence::create(DelayTime::create(1.5), EaseQuinticActionOut::create(MoveTo::create(1, target_position)), nullptr));
        }
    }
   
    if (_next_info.activity_id != -1) {

        this->scheduleOnce([=](float dt) {
            this->readActivityName(_next_info.name);
            }, 2, "read_activity");
    }
}

void MSActivityEnd::onEnter()
{
    ui::Layout::onEnter();
    
    if (_is_animate_next)
        ui::Helper::doLayout(this);

	auto closeLayout = ImageView::create("mjstory/close_game.png");
	this->addChild(closeLayout, 1000000000);
	closeLayout->setPosition(cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height + 50));
	closeLayout->setAnchorPoint(cocos2d::Point::ANCHOR_TOP_RIGHT);

	auto lblClose = Label::createWithTTF("Close?", "fonts/Montserrat-SemiBold.ttf", 16);
//    lblClose->enableOutline(Color4B::WHITE, 1);
	lblClose->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	lblClose->setPosition(Vec2(closeLayout->getContentSize().width / 2, -10));
	lblClose->setTextColor(Color4B(Color3B(38, 176, 230)));
	lblClose->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
	closeLayout->addChild(lblClose);
	lblClose->setVisible(false);

	auto button = cocos2d::ui::Button::create("mjstory/papgemenucontrol_btclose.png");
	button->setAnchorPoint(cocos2d::Point::ANCHOR_MIDDLE);
	button->setPositionX(closeLayout->getContentSize().width / 2);
	button->setPositionY(10);
	button->setLocalZOrder(-1);
	button->setOpacity(0);
	button->setScale(1.5f);
    button->setName("button_click_close_pop");
	//button->setPosition(cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width, cocos2d::Director::getInstance()->getVisibleSize().height));
	button->addClickEventListener([=](Ref * sender)
    {
        if (!close_button_data.onMove){
			if (close_button_data.onBot){
				//this->onGameOver(true);
                this->unschedule("read_activity");
                this->unschedule("delay_read_activity_name");
				close_button_data.onMove = true;
               /* if (auto aPopupStart = utils::findChild(Director::getInstance()->getRunningScene(), "popup_start")) {
                    aPopupStart->removeFromParent();
                    x = true;
                }*/

                if (auto wrapper = utils::findChild(Director::getInstance()->getRunningScene(), "tmp_popup_start")) {
                    wrapper->removeFromParent();
                }

                if (auto popup_start = utils::findChild(Director::getInstance()->getRunningScene(), "wrapper"))
                {
                    popup_start->removeFromParent();
                }
                else
                {
                    _delegate ? _delegate->onBackToLesson() : (void) nullptr;
                }
                
				cocos2d::experimental::AudioEngine::stopAll();
                
                if (CONFIG_MANAGER->isVietnameseStories())
                {
                    auto lesson = ms::LessonManager::getInstance().getLessonById(_info.lesson_id);
                    int activities_size = (int)StoryDataModel::getInstance()->getActivitiesOfLesson(_info.lesson_id).size();
                    int currentActivityID = _info.activity_id;
                    
                    MSActivityWrapper::pushCleverTapTrackingData_Vmonkey(lesson, activities_size, currentActivityID);
                }
			}
			else{
                close_button_data.onMove = true;
				closeLayout->runAction(Sequence::create(MoveTo::create(0.25f, cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height))
					, CallFunc::create([=](){
					lblClose->setVisible(true);
					close_button_data.onBot = true;
					close_button_data.onMove = false;
					this->scheduleOnce([=](float dt){
						close_button_data.onMove = true;
						close_button_data.onBot = false;
						closeLayout->runAction(Sequence::create(MoveTo::create(0.25f, cocos2d::Point(cocos2d::Director::getInstance()->getVisibleSize().width - 20, cocos2d::Director::getInstance()->getVisibleSize().height + 50))
							, CallFunc::create([=](){
							close_button_data.onMove = false;
							lblClose->setVisible(false);
						}), nullptr));
					}, 3.0f, "back_to_top");
				}), nullptr));
			}

		}
	});
	closeLayout->addChild(button, 1000000000);
    if (CONFIG_MANAGER->isAutoPlay()) {
        this->scheduleOnce([=](float dt) {
            onContinue(nullptr);
            }, 1.5f, "continue_play");
    }
}

void MSActivityEnd::onBackToLesson(cocos2d::Ref *sender) {
    this->unschedule("read_activity");
    _delegate ? _delegate->onBackToLesson() : (void) nullptr;
    cocos2d::experimental::AudioEngine::stopAll();
}

void MSActivityEnd::onContinue(cocos2d::Ref* sender) {
    this->unschedule("read_activity");
    if (_delegate)
    {
        if (auto buttonCloseGame = utils::findChild<ui::Button*>(this, "button_click_close_pop"))
        {
            buttonCloseGame->setTouchEnabled(false);
        }
    }

    auto button_play = utils::findChild(this, "button_play");
    if (button_play) {
        button_play->removeFromParent();
        auto indicator = utils::findChild(this, "indicator");
        indicator->setVisible(true);
        indicator->runAction(RepeatForever::create(RotateBy::create(0.5, 360)));

        _delegate ? _delegate->onContinueLearn(_info) : (void) nullptr;
        }
    }

void MSActivityEnd::setCallbackActivityName()
{
    _callbackActivityName = [=](std::string act_name) {
        if (CONFIG_MANAGER->isVietnameseStories()) {
            if (this->isScheduled("delay_read_activity_name"))
            {
                this->unschedule("delay_read_activity_name");
            }
            this->scheduleOnce([](float dt) {
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_play_activity_name);
                }, 0.75, "delay_read_activity_name");

            return;
        }
        auto read_activity = cocos2d::experimental::AudioEngine::play2d("sounds/activities_name/activity.mp3");
        if (read_activity == -1) {
            if (this->isScheduled("delay_read_activity_name"))
            {
                this->unschedule("delay_read_activity_name");
            }
            this->scheduleOnce([](float dt) {
                Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_play_activity_name);
                }, 0.25, "delay_read_activity_name");
            return;
        }

        this->schedule([=](float) {
            _countContiue++;
            if(_countContiue >= COUTN_CONTIUE)
            {
                this->scheduleOnce([](float dt) {
                    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_play_activity_name);
                }, 0.25, "delay_read_activity_name");
            }
        }, 1, COUTN_CONTIUE + 1, 1, "countForGotoCallBack");

        cocos2d::experimental::AudioEngine::setFinishCallback(read_activity, [=](int, std::string) {

            if (_isExist)
            {
                return;
            }

            auto order = _info.order + 1;
            if (_is_animate_next) {
                if (_next_info.activity_id != -1) {
                    order = _next_info.order + 1;
                }
            }
            auto activity_number = StringUtils::format("sounds/activities_name/%d.mp3", order);
            auto read_number = cocos2d::experimental::AudioEngine::play2d(activity_number);

            if (read_number == -1) {
                if (this->isScheduled("delay_read_activity_name"))
                {
                    this->unschedule("delay_read_activity_name");
                }
                this->scheduleOnce([](float dt) {
                    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_play_activity_name);
                    }, 0.25, "delay_read_activity_name");
                return;
            }

            cocos2d::experimental::AudioEngine::setFinishCallback(read_number, [=](int, std::string) {
                if (_isExist)
                {
                    return;
                }
                auto name = mj::helper::getActivitySound(act_name);
                auto read_name = cocos2d::experimental::AudioEngine::play2d(name);

                if (read_name == -1) {
                    if (this->isScheduled("delay_read_activity_name"))
                    {
                        this->unschedule("delay_read_activity_name");
                    }
                    this->scheduleOnce([](float dt) {
                        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_play_activity_name);
                        }, 0.25, "delay_read_activity_name");
                    return;
                }

                if (!_is_animate_next) {
                    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_play_activity_name);
                    cocos2d::experimental::AudioEngine::setFinishCallback(read_name, [=](int, std::string) {
                        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(key_event_play_activity_name);
                        });
                }
                });
            });
    };
}

void MSActivityEnd::readActivityName(std::string act_name) {
    
    if (_callbackActivityName)
    {
        _callbackActivityName(act_name);
    }

}

ui::Widget::ccWidgetClickCallback MSActivityEnd::onLocateClickCallback(const std::string & callback_name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(MSActivityEnd, onBackToLesson),
        CLICK_MAP(MSActivityEnd, onContinue)
    };
    
    if (function_map.count(callback_name) > 0) {
        return function_map.at(callback_name);
    }
    
    return nullptr;
}

void MSActivityEnd::onExit() {
    _isExist = true;
    _callbackActivityName = nullptr;
    ui::Layout::onExit();
}

