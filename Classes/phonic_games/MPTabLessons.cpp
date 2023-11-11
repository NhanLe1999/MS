//
//  MPTabLessons.cpp

#include "MPTabLessons.h"
#include "network/HttpClient.h"
#include "network/CCDownloader.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "MJHelper.h"
#include "MPTabGame.h"
USING_NS_CC;
INITIALIZE_READER(MPTabLessons);

using namespace cocos2d::network;
using namespace rapidjson;

#define CSB_LESSSONS "csb/new_ui/TabLesson.csb"
std::once_flag mp_tab_lessons_reader;

#define BASE_URL "http://api.daybehoc.com"
#define APP_ID 52
#define DEVICE_ID 123

MPTabLessons * MPTabLessons::createTab() {
	std::call_once(mp_tab_lessons_reader, [] {
		REGISTER_CSB_READER(MPTabLessons);
	});

	auto tab = reinterpret_cast<MPTabLessons *>(CSLoader::createNodeWithVisibleSize(CSB_LESSSONS));
	if (tab) {
		tab->didLoadFromCSB();
		return tab;
	}

	CC_SAFE_DELETE(tab);
	return nullptr;
}

void MPTabLessons::didLoadFromCSB() {

}

void MPTabLessons::onEnter() {
	ui::Layout::onEnter();
	fetchCategories();
	fetchLessonsFromAPI([=](bool success) {
		//filterLessons();
	});
}

int MPTabLessons::numberOfCells() {
	return (int)_lessons.size();
}

int MPTabLessons::numberOfColumns() {
	return (int)((collectionSize().width + 10) / (modelSize().width + 10));
}

//cocos2d::ui::Widget * MPTabLessons::cellForPosition(int row, int col) {
//    auto index = row * numberOfColumns() + col;
//    if (_ui_source[index]) {
//    } else {
//		auto lesson = cocos2d::ui::Widget::create();
//		lesson->addClickEventListener([=](cocos2d::Ref * sender) {
//		
//		});
//        _ui_source[index] = lesson;
//    }
//    
//    return _ui_source[index];
//}

cocos2d::Size MPTabLessons::modelSize() {
	return Size(250, 250);
}

cocos2d::Size MPTabLessons::collectionSize() {
	auto collection_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "collection_layout");
	auto collection = cocos2d::utils::findChild<cocos2d::ui::ScrollView *>(collection_layout, "collection");
	return collection->getContentSize();
}


double MPTabLessons::percentScrollWhenReload() {
	auto total_row = ((int)_lessons.size() - 1) / numberOfColumns() + 1;
	auto row = (_saved_current_row - 1) / numberOfColumns() + 1;
	auto percent = (row - 1) * 100.f / (total_row - 1);
	return percent;
}

void MPTabLessons::onLearnLesson(mj::model::LessonInfo lesson) {
	
	if (_delegate) {
		_delegate->onClickedLessonDelegate(lesson);
	}	
}



void MPTabLessons::onBackToLesson() {
	Director::getInstance()->getRunningScene()->removeChildByName("popup_start");
}

void MPTabLessons::filterLessons(int category_id)
{
	
	getAllLessonsByGrade(category_id);
	//_lessons = _all_lessons;

	auto collection_layout = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "collection_layout");
	auto collection = cocos2d::utils::findChild<cocos2d::ui::ScrollView *>(collection_layout, "collection");
	collection->removeAllChildren();
	collection->setBounceEnabled(true);
	
	auto width_item = modelSize().width;
	auto heigh_item = modelSize().height;
	auto column = 3;//(int)screen_size.width/width_item;
	auto index_x = 0;
	auto index_y = 0;

	collection->setInnerContainerSize(cocos2d::Size(collection->getContentSize().width, (_lessons.size() / column + 4)*heigh_item + 10));
	//create button
	for (int i = 0; i < _lessons.size(); i++) {

		auto btn = cocos2d::ui::Button::create("phonics_btnext.png");
		btn->setScale9Enabled(true);
		btn->setCapInsets(Rect(21, 21, 1, 1));
		btn->setContentSize(cocos2d::Size(modelSize()));
		btn->setZoomScale(0);
		btn->setName("game_btn");

		index_x = i % column;
		index_y = i / column;
		auto position = Vec2((index_x + 0.5)*(width_item + 50), collection->getInnerContainerSize().height - (index_y + 0.5) * (heigh_item + 20 ));
		auto wrapper = cocos2d::ui::Widget::create();
		wrapper->setContentSize(btn->getContentSize() + cocos2d::Size(10, 10));
		btn->setPosition(wrapper->getContentSize() / 2);
		wrapper->addChild(btn);
		wrapper->setPosition(position);
		collection->addChild(wrapper);

		auto label = cocos2d::ui::Text::create(_lessons[i].name, "fonts/KGWHATTHETEACHERWANTS.TTF", 50);
		label->setPosition(Vec2(btn->getContentSize()/ 2));
		label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE);
		label->setTextAreaSize(btn->getContentSize());
		btn->addChild(label);
		label->setTextColor(Color4B::WHITE);
		btn->setUserObject(label);
		btn->setUserData(&_lessons[i]);

		btn->addClickEventListener([=](Ref* ref) {
			//auto _btn = (ui::Button *)(ref);
			auto lesson_info = static_cast<mj::model::LessonInfo *>(btn->getUserData());
			this->onLearnLesson(*(lesson_info));
		});
	}

	collection->jumpToTop();

}

void MPTabLessons::reloadData() {
	CCLOG("reload tab lessons");
}


std::vector<mj::model::LessonInfo> MPTabLessons::getAllLessonsByGrade(int grade_id)
{
	std::vector<mj::model::LessonInfo> t;
	std::copy_if(_all_lessons.begin(), _all_lessons.end(), std::back_inserter(t), [grade_id](mj::model::LessonInfo l) {
		return l.category_id == grade_id;
	});

	_lessons = t;
	std::sort(_lessons.begin(), _lessons.end(), [](mj::model::LessonInfo a, mj::model::LessonInfo b) -> bool {
		return a.lesson_order < b.lesson_order;
	});
	return _lessons;
}

std::vector<mj::model::LessonInfo> MPTabLessons::getAllLessons()
{
	return _all_lessons;
}

void MPTabLessons::setDelegate(MPTabLessonsDelegate * delegate)
{
	this->_delegate = delegate;
}

cocos2d::ui::Widget::ccWidgetClickCallback MPTabLessons::onLocateClickCallback(const std::string & callback_name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		{ "onClickGrade",CC_CALLBACK_1(MPTabLessons::onClickedGrade,this) }
	};

	if (function_map.find(callback_name) != function_map.end()) {
		return function_map.at(callback_name);
	}
	return nullptr;
}

void MPTabLessons::onClickedGrade(cocos2d::Ref * ref)
{
	auto button = static_cast<cocos2d::ui::Button *>(ref);
	if (button) {
		auto name = button->getName();
	}
}


void MPTabLessons::fetchLessonsFromAPI(std::function<void(bool)> callback)
{
	HttpRequest* request = new HttpRequest();
	std::stringstream ss;

	ss << BASE_URL << "/api/v1/mobile/utils/appinfov2?app_id=" << APP_ID << "&device_id=" << DEVICE_ID <<
		"&os=win32&info&app_ver=1&device_type=2&lang_id=1&lang=vi-VN&lang_id=4&api_ver_audio_book=1536137456&api_ver_story=1536137456&version_game_number=0&version_lesson_number=0";

	request->setUrl(ss.str());
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

		// create zip file from response
		std::string dataBuffer = std::string(responseData->begin(), responseData->end());
		auto zipFile = cocos2d::ZipFile::createWithBuffer(dataBuffer.c_str(), dataBuffer.size());


		std::string fileName = zipFile->getFirstFilename();
		ssize_t size = 0;
		auto fileData = zipFile->getFileData(fileName, &size);

		std::string buffer;
		buffer.assign((const char *)fileData, size);


		if (d.Parse(buffer.c_str()).HasParseError()) {
			// failed
			if (callback)
				callback(false);
			return;
		}

		if (!(d.HasMember("lesson_list") && d["lesson_list"].IsObject())) {
			if (callback)
				callback(false);
			return;
		}

		_all_lessons.clear();
		auto dataArray = d["lesson_list"]["list"].GetArray();
		for (auto i = 0; i < dataArray.Size(); i++) {
			mj::model::LessonInfo li;
			auto lessionData = dataArray[i].GetObject();
			if (lessionData.HasMember("id") &&
				lessionData.HasMember("order_by") &&
				lessionData.HasMember("game_category_id") &&
				lessionData.HasMember("path_thumb") &&
				lessionData.HasMember("title")) {

				li.lesson_id = lessionData["id"].GetInt();
				li.category_id = lessionData["game_category_id"].GetInt();
				li.lesson_order = lessionData["order_by"].GetInt();
				li.name = lessionData["title"].GetString();
				li.thumbnail_url = lessionData["path_thumb"].GetString();
			}
			_all_lessons.push_back(li);
		}

		if (callback)
			callback(true);
	});

	HttpClient::getInstance()->send(request);
	request->release();

	std::sort(_all_lessons.begin(), _all_lessons.end(), [](mj::model::LessonInfo a, mj::model::LessonInfo b) -> bool {
		return a.lesson_order < b.lesson_order;
	});
}

void MPTabLessons::fetchCategories()
{
	HttpRequest* request = new HttpRequest();
	std::stringstream ss;
	ss << BASE_URL << "/api/v1/mobile/categories/list?app_id=" << APP_ID << "&device_id=" << DEVICE_ID <<
		"&os=win32&info&app_ver=1&device_type=2&lang_id=1&lang=vi-VN";
	request->setUrl(ss.str());
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([=](HttpClient* client, HttpResponse* response) {
		if (response->getResponseCode() != 200) {
			// failed

			return;
		}

		Document d;
		auto responseData = response->getResponseData();
		if (d.Parse(std::string(responseData->begin(), responseData->end()).c_str()).HasParseError()) {
			// failed
			return;
		}

		if (!(d.HasMember("data") && d["data"].IsArray())) {
			return;
		}
		auto grade_choose = cocos2d::utils::findChild<cocos2d::ui::Layout *>(this, "grade_choose");
		grade_choose->removeAllChildren();
		auto dataArray = d["data"].GetArray();
		for (auto i = 0; i < dataArray.Size(); i++) {
			auto categoryData = dataArray[i].GetObject();
			auto name = categoryData["name"].GetString();
			auto id = categoryData["id"].GetInt();
			auto btn = cocos2d::ui::Button::create("mphonics_btnext.png");
			btn->setScale9Enabled(true);
			btn->setCapInsets(Rect(21, 21, 1, 1));
			btn->setContentSize(cocos2d::Size(150, 100));
			btn->setZoomScale(0);
			btn->setName("game_btn");
			btn->setTag(id);
			auto wrapper = cocos2d::ui::Widget::create();
			wrapper->setContentSize(btn->getContentSize() + cocos2d::Size(10, 10));
			wrapper->setPosition(cocos2d::Vec2((i + 0.5) * (btn->getContentSize().width + 30), grade_choose->getContentSize().height / 2));
			btn->setPosition(wrapper->getContentSize() / 2);
			wrapper->addChild(btn);
			wrapper->setTag(id);
			grade_choose->addChild(wrapper);

			auto label = cocos2d::Label::create(name, "fonts/KGWHATTHETEACHERWANTS.TTF", 40);
			label->setPosition(10, btn->getContentSize().height / 2);
			label->setAnchorPoint(cocos2d::Vec2::ANCHOR_MIDDLE_LEFT);
			btn->addChild(label);
			label->setTextColor(Color4B::WHITE);
			btn->setUserObject(label);

			btn->addClickEventListener([=](Ref* ref) {
				auto id = static_cast<cocos2d::ui::Button*>(ref)->getTag();
				if (m_currentCategory == id)
					return;
				auto w = grade_choose->getChildByTag(m_currentCategory);
				if (w) {
					auto b = w->getChildByTag(m_currentCategory);
					if (b) b->setScale(1.0f);
				}
					
				btn->setScale(1.25f);
				m_currentCategory = id;
				filterLessons(m_currentCategory);

			});
		}
	});

	HttpClient::getInstance()->send(request);
	request->release();
}
