#include "MSPopularSearchItem.h"
#include "RemoteImageView.h"

USING_NS_CC;
INITIALIZE_READER(MJPopularSearchItem);

std::once_flag mj_popular_search_item_reader;

MJPopularSearchItem* MJPopularSearchItem::createItem(mj::model::MaterialPopularInfo info) {
	std::call_once(mj_popular_search_item_reader, [] {
		REGISTER_CSB_READER(MJPopularSearchItem);

		});

	auto item = reinterpret_cast<MJPopularSearchItem*>(CSLoader::createNode("csb/new_ui/material/PopularSearchItem.csb"));
	if (item) {
		item->didLoadFromCSB(info);
		return item;
	}

	CC_SAFE_DELETE(item);
	return nullptr;
}

MJPopularSearchItem * MJPopularSearchItem::createItem(mj::model::PopularSearchAudiobookInfo info)
{
	std::call_once(mj_popular_search_item_reader, [] {
		REGISTER_CSB_READER(MJPopularSearchItem);

	});

	auto item = reinterpret_cast<MJPopularSearchItem*>(CSLoader::createNode("csb/new_ui/material/PopularSearchItem.csb"));
	if (item) {
		item->didLoadFromCSB(info);
		return item;
	}

	CC_SAFE_DELETE(item);
	return nullptr;
}

MJPopularSearchItem * MJPopularSearchItem::createItem(mj::model::PopularSearchStoryInfo info)
{
	std::call_once(mj_popular_search_item_reader, [] {
		REGISTER_CSB_READER(MJPopularSearchItem);

	});

	auto item = reinterpret_cast<MJPopularSearchItem*>(CSLoader::createNode("csb/new_ui/material/PopularSearchItem.csb"));
	if (item) {
		item->didLoadFromCSB(info);
		return item;
	}

	CC_SAFE_DELETE(item);
	return nullptr;
}

void MJPopularSearchItem::didLoadFromCSB(mj::model::MaterialPopularInfo info) {
	_info = info;

	_thumbnail = utils::findChild<ui::ImageView*>(this, "thumbnail");
	auto remoteView = RemoteImageView::createImage(_info.thumb, [=](RemoteImageView* img, bool is_success) {
		if (is_success) {
			img->setContentSize(_thumbnail->getContentSize());
			img->setOpacity(0);
			img->runAction(FadeIn::create(0.25));
		}
    }, RemoteImageView::Type::OTHER);
    
	remoteView->setAnchorPoint(_thumbnail->getAnchorPoint());
	remoteView->setPosition(_thumbnail->getPosition());
	//remoteView->setName("thumb");
	remoteView->setContentSize(_thumbnail->getContentSize());
	utils::findChild(this, "Layout")->addChild(remoteView, -1);
	_thumbnail->setVisible(false);

	_keyword = utils::findChild<ui::Text*>(this, "name");
	_keyword->setString(_info.keyword);
	
	_footer = utils::findChild<ui::ImageView*>(this, "footer");
//    _footer->setContentSize(Size(_keyword->getContentSize().width+27, _footer->getContentSize().height));
    this->setNewFooter("mjstory/material/mat_panel_search_popular.png");
}

void MJPopularSearchItem::setNewFooter(const std::string& texture){
    _footer->loadTexture(texture);
    _footer->setContentSize(Size(this->getContentSize().width + 6.f, 44));
    _footer->setAnchorPoint(Point(0.5, 0));
    _footer->setPosition(Point(this->getContentSize().width/2 - 0.5f, -2.5f));
    
    if (auto name = utils::findChild<ui::Text *>(_footer, "name")) {
        name->setPosition(Point(this->getContentSize().width/2 - name->getContentSize().width/2, 20));
    }
}

void MJPopularSearchItem::didLoadFromCSB(mj::model::PopularSearchAudiobookInfo info)
{
	_audiobookInfo = info;

	_thumbnail = utils::findChild<ui::ImageView*>(this, "thumbnail");
	auto remoteView = RemoteImageView::createImage(_audiobookInfo.thumbnailUrl, [=](RemoteImageView* img, bool is_success) {
		if (is_success) {
			img->setContentSize(_thumbnail->getContentSize());
			img->setOpacity(0);
			img->runAction(FadeIn::create(0.25));
		}
	}, RemoteImageView::Type::OTHER);

	remoteView->setAnchorPoint(_thumbnail->getAnchorPoint());
	remoteView->setPosition(_thumbnail->getPosition());
	//remoteView->setName("thumb");
	remoteView->setContentSize(_thumbnail->getContentSize());
	utils::findChild(this, "Layout")->addChild(remoteView, -1);
	_thumbnail->setVisible(false);

	_keyword = utils::findChild<ui::Text*>(this, "name");
	_keyword->setString(_audiobookInfo.keyword);

	_footer = utils::findChild<ui::ImageView*>(this, "footer");
//    _footer->setContentSize(Size(_keyword->getContentSize().width + 27, _footer->getContentSize().height));
//
//    if (auto footer = utils::findChild<ui::ImageView*>(this, "footer")) {
//        footer->loadTexture("mjstory/material/mat_duc/hd/popular_search_item/popular_search_footer_listening.png");
//    }
    this->setNewFooter("mjstory/material/mat_panel_search_popular_2.png");

	if (auto icon = utils::findChild<ui::ImageView*>(this, "icon")) {
		icon->loadTexture("mjstory/material/mat_duc/hd/popular_search_item/icon_listening.png");
	}
}

void MJPopularSearchItem::didLoadFromCSB(mj::model::PopularSearchStoryInfo info)
{
	_storyInfo = info;

	_thumbnail = utils::findChild<ui::ImageView*>(this, "thumbnail");
	auto remoteView = RemoteImageView::createImage(_storyInfo.thumbnailUrl, [=](RemoteImageView* img, bool is_success) {
		if (is_success) {
			img->setContentSize(_thumbnail->getContentSize());
			img->setOpacity(0);
			img->runAction(FadeIn::create(0.25));
		}
	}, RemoteImageView::Type::OTHER);

	remoteView->setAnchorPoint(_thumbnail->getAnchorPoint());
	remoteView->setPosition(_thumbnail->getPosition());
	//remoteView->setName("thumb");
	remoteView->setContentSize(_thumbnail->getContentSize());
	utils::findChild(this, "Layout")->addChild(remoteView, -1);
	_thumbnail->setVisible(false);

	_keyword = utils::findChild<ui::Text*>(this, "name");
	_keyword->setString(_storyInfo.keyword);

	_footer = utils::findChild<ui::ImageView*>(this, "footer");
//    _footer->setContentSize(Size(_keyword->getContentSize().width + 27, _footer->getContentSize().height));
//
//    if (auto footer = utils::findChild<ui::ImageView*>(this, "footer")) {
//        footer->loadTexture("mjstory/material/mat_duc/hd/popular_search_item/popular_search_footer_reading.png");
//    }
    this->setNewFooter("mjstory/material/mat_panel_search_popular_1.png");

	if (auto icon = utils::findChild<ui::ImageView*>(this, "icon")) {
		icon->loadTexture("mjstory/material/mat_duc/hd/popular_search_item/icon_reading.png");
	}
}

void MJPopularSearchItem::onEnter() {
	ui::Layout::onEnter();
}

std::string MJPopularSearchItem::getKeyword() {
	return _keyword->getString();
}

cocos2d::ui::Widget::ccWidgetClickCallback MJPopularSearchItem::onLocateClickCallback(const std::string& callback_name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
	{
		CLICK_MAP(MJPopularSearchItem, onSelectPopularSearch),
	};


	if (function_map.count(callback_name) > 0)
	{
		return function_map.at(callback_name);
	}

	return nullptr;
}

void MJPopularSearchItem::onSelectPopularSearch(cocos2d::Ref* sender) {
	CCLOG("CLICKED POP SEARCH!");
}
