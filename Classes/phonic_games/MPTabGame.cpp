//
//  MPTabGames.cpp

#include "MPTabGame.h"
#include "network/HttpClient.h"
#include "network/CCDownloader.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "MJHelper.h"

USING_NS_CC;
INITIALIZE_READER(MPTabGames);

using namespace cocos2d::network;
using namespace rapidjson;

#define CSB_TAB_GAMES "csb/new_ui/TabGame.csb"
std::once_flag mp_tab_games_reader;

#define BASE_URL "http://api.daybehoc.com"
#define APP_ID 52
#define DEVICE_ID 123

MPTabGames * MPTabGames::createTab(mj::model::LessonInfo info) {
    std::call_once(mp_tab_games_reader, [] {
        REGISTER_CSB_READER(MPTabGames);
    });

    auto tab = reinterpret_cast<MPTabGames *>(CSLoader::createNodeWithVisibleSize(CSB_TAB_GAMES));
    if (tab) {
        tab->didLoadFromCSB(info);
        return tab;
    }
    
    CC_SAFE_DELETE(tab);
    return nullptr;
}

mj::model::ActInfo MPTabGames::getNextActivity(mj::model::ActInfo info)
{
	mj::model::ActInfo r;
	r.activity_id = -1;
	r.lesson_id = -1;

	auto a_it = std::find_if(std::begin(_activities), std::end(_activities), [info](mj::model::ActInfo a) {
		return a.activity_id == info.activity_id;
	});

	if (a_it == std::end(_activities)) {
		return r;
	}

	a_it++;
	if (a_it == std::end(_activities)) {
		return r;
	}

	if (a_it != std::end(_activities))
		r = *a_it;
	return r;
}

void MPTabGames::setDelegate(MPTabGamesDelegate * _delegate)
{
	this->_delegate = _delegate;
}


void MPTabGames::didLoadFromCSB(mj::model::LessonInfo info) {
	lesson_info = info;
	auto root_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "root_layout");
	auto text_lb = cocos2d::utils::findChild<cocos2d::ui::Text *>(root_layout, "lesson_info");
	text_lb->setString(info.name);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	//back button
	auto btn_close = ui::Button::create("phonics_btnext.png");
	btn_close->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	btn_close->setPosition(Vec2(visibleSize) - Point(50, 50));
	addChild(btn_close, 3);
	btn_close->addClickEventListener([=](Ref* ref) {
		this->removeFromParent();
	});

	auto game_list = cocos2d::utils::findChild<cocos2d::ui::ListView *>(root_layout, "game_list");
	game_list->setMagneticAllowedOutOfBoundary(true);
	game_list->setScrollDuration(0.5f);
	game_list->setBounceEnabled(true);
	game_list->setInertiaScrollEnabled(true);
	game_list->setFocused(true);
}

void MPTabGames::onEnter() {
    ui::Layout::onEnter();
	fetchActivitiesFromAPI([=](bool success) {
		if (success) {
			filterActivities();
		}
	});
}

int MPTabGames::numberOfCells() {
    return (int)_activities.size();
}



void MPTabGames::fetchActivitiesFromAPI(std::function<void(bool)> callback)
{
	HttpRequest* request = new HttpRequest();
	std::stringstream ss;

	ss << BASE_URL << "/api/v1/mobile/activities/list?app_id=" << APP_ID << "&device_id=" << DEVICE_ID <<
	"os=ios&info=2.2.2%7CiOS%7CiPad%204%7C10.3.3%7C5714&subversion=2.2.2&app_ver=1&key_has=fDUyREU3ODlELUVDREQtNDQxOS1CRTkwLTNEQ0QzN0Q5MjU4Q3xDMTdCQkY1My1DQTExLTQ3MzQtQkUwOS0wOThGRkJGRkE5MjU%3D&device_type=2&lang=vi-VN&lang_id=1&lesson_id=" << lesson_info.lesson_id << "&sv=12";
	request->setUrl(ss.str());
	//CCLOG(ss.str().c_str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
		if (response->getResponseCode() != 200) {
			// failed
			if (callback)
				callback(false);
			return;
		}

		Document d;
		auto responseData = response->getResponseData();
		if (d.Parse(std::string(responseData->begin(), responseData->end()).c_str()).HasParseError()) {
			// failed
			if (callback)
				callback(false);
			return;
		}

		if (!(d.HasMember("data") && d["data"].IsArray())) {
			if (callback)
				callback(false);
			return;
		}
		
		rapidjson::Value& activitiesData = d["data"][0];
		if (!(activitiesData.HasMember("list_ac") && activitiesData["list_ac"].IsArray())) {
			if (callback)
				callback(false);
			return;
		}
		_activities.clear();
		auto dataArray = activitiesData["list_ac"].GetArray();
		for (auto i = 0; i < dataArray.Size(); i++) {
			mj::model::ActInfo ai;
			auto activityData = dataArray[i].GetObject();
			if (activityData.HasMember("activity_id") &&
				activityData.HasMember("order_by") &&
				activityData.HasMember("game_id") &&
				activityData.HasMember("url_download") &&
				activityData.HasMember("name")) {

				ai.activity_id = activityData["activity_id"].GetInt();
				ai.sub_id = activityData["game_id"].GetInt();
				ai.order = activityData["order_by"].GetInt();
				ai.name = activityData["name"].GetString();
				ai.data_zip = activityData["url_download"].GetString();
			}
			_activities.push_back(ai);
		}

		std::sort(_activities.begin(), _activities.end(), [](mj::model::ActInfo a, mj::model::ActInfo b) -> bool {
			return a.order < b.order;
		});
		if (callback)
			callback(true);
	});

	HttpClient::getInstance()->send(request);
	request->release();
}

mj::model::ActInfo MPTabGames::getActivityById(int activity_id)
{
	auto it = std::find_if(std::begin(_activities), std::end(_activities), [activity_id](mj::model::ActInfo l) {
		return l.activity_id == activity_id;
	});

	mj::model::ActInfo ai;
	ai.activity_id = -1;

	if (it != std::end(_activities)) {
		ai = *it;
	}

	return ai;
}

cocos2d::Size MPTabGames::modelSize() {
	return Size(200, 260);
}

cocos2d::Size MPTabGames::collectionSize() {
    return utils::findChild(this, "collection")->getContentSize();
}


void MPTabGames::onBackToLesson() {
    Director::getInstance()->getRunningScene()->removeChildByName("popup_start");
}

void MPTabGames::filterActivities()
{
	auto root_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "root_layout");
	auto game_list = cocos2d::utils::findChild<cocos2d::ui::ListView *>(root_layout, "game_list");
	for (int i = 0; i < _activities.size(); ++i) {

		auto btn = cocos2d::ui::Button::create("phonics_btnext.png");
		btn->setScale9Enabled(true);
		btn->setCapInsets(Rect(21, 21, 1, 1));
		btn->setContentSize(cocos2d::Size(game_list->getContentSize().width - 20, 200));
		btn->setZoomScale(0);
		btn->setName("game_btn");

		auto wrapper = cocos2d::ui::Widget::create();
		wrapper->setContentSize(btn->getContentSize() + cocos2d::Size(10, 10));
		btn->setPosition(wrapper->getContentSize() / 2);
		wrapper->addChild(btn);
		game_list->addChild(wrapper);

		auto info = _activities[i];
		auto label = cocos2d::ui::Text::create(info.name, "fonts/KGWHATTHETEACHERWANTS.TTF", 50);
		label->setPosition(Vec2(15, btn->getContentSize().height / 2));
		label->setTextAreaSize(btn->getContentSize());
		label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
		btn->addChild(label);
		label->setTextColor(Color4B::WHITE);
		btn->setUserObject(label);
		btn->setUserData(&_activities[i]);

		btn->addClickEventListener([=](Ref* ref) {
			auto act_info = (mj::model::ActInfo *) btn->getUserData();
			if (_delegate) {
				_delegate->onClickedActiviyDelegate(*act_info);
			}
		});
	}
    game_list->setInnerContainerSize(Size(_activities.size() * game_list->getItem(0)->getContentSize().width, game_list->getItem(0)->getContentSize().height ));
   	game_list->jumpToLeft();
}

