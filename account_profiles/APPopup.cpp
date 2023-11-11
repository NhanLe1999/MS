//
//  AccountPopup.cpp
//  MonkeyMath
//
//  Created by NAM on 5/16/18.
//

#include "APPopup.h"
#include "StoryLanguageManager.h"

USING_NS_CC;
INITIALIZE_READER(AccountPopup);

std::once_flag account_popup_reader_flag;

AccountPopup * AccountPopup::createView(APPopupInfo info) {
    std::call_once(account_popup_reader_flag, [] {
        REGISTER_CSB_READER(AccountPopup);
    });
    
    auto view = reinterpret_cast<AccountPopup *>(CSLoader::createNodeWithVisibleSize("csb/account/AccountPopup.csb"));
    if (view) {
        view->didLoadFromCSB(info);
        return view;
    }
    
    CC_SAFE_DELETE(view);
    return view;
}


AccountPopup * AccountPopup::show(AccountPopup::APPopupInfo info, std::function<void()> left_callback /*nullptr*/, std::function<void()> right_callback /*nullptr*/, int z_order /*0*/) {
    auto popup = AccountPopup::createView(info);
	if (left_callback == nullptr) popup->_is_left_null_callback = true;
	if (right_callback == nullptr) popup->_is_right_null_callback = true;
    popup->_right_callback = right_callback;
    popup->_left_callback = left_callback;
    if (z_order == 0) {
        z_order = INT_MAX - 1;
    }
    Director::getInstance()->getRunningScene()->addChild(popup, z_order);
    return popup;
}


void AccountPopup::didLoadFromCSB(APPopupInfo info) {
    this->setTouchEnabled(true);
    
    auto content = utils::findChild(this, "bg");
    auto content_width = content->getContentSize().width - 50;
    auto title = utils::findChild<ui::Text *>(content, "title");
    auto title_left = utils::findChild<ui::Text *>(content, "text_left");

	title->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf");
	title_left->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf");

    title_left->setString(info.left);
    auto bt_left = title_left->getParent();
    if (title_left->getContentSize().width > bt_left->getContentSize().width) {
        bt_left->setContentSize(Size(title_left->getContentSize().width + 50, bt_left->getContentSize().height));
    }
    
    auto title_right = utils::findChild<ui::Text *>(content, "text_right");
    title_right->setString(info.right);
	title_right->setFontName(LANGUAGE_MANAGER->getDisplayLangId() == LANGUAGE_THAI_CODE ? "fonts/leelawdb.ttf" : "fonts/Montserrat-Medium.ttf");
    auto bt_right = title_right->getParent();
    if (title_right->getContentSize().width > bt_right->getContentSize().width) {
        bt_right->setContentSize(Size(title_right->getContentSize().width + 50, bt_right->getContentSize().height));
    }
    
    if (bt_left->getContentSize().width + bt_right->getContentSize().width > 500) {
        content_width = bt_left->getContentSize().width + bt_right->getContentSize().width + 200;
    }

//	text_left
    title->setString(info.title);
    
    /*auto content_height = title->getContentSize().height + message->getContentSize().height + bottom->getContentSize().height + listview->getItemsMargin() * 2;
    
    content->setContentSize(Size(content_width + 50, content_height + 50));*/
    
    ui::Helper::doLayout(this);
    
    if (info.left.empty() || info.left == "") {
		title_left->getParent()->removeFromParent();
        title_right->getParent()->setPositionX(content->getContentSize().width/2);
    }
    
    if (info.right.empty() || info.right == "") {
        title_right->getParent()->removeFromParent();
		title_left->getParent()->setPositionX(content->getContentSize().width / 2);
    }
}

void AccountPopup::onEnter() {
    ui::Layout::onEnter();
    doAppear();
	/*auto runani = CSLoader::createTimeline("csb/account/AccountPopup.csb");
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);*/
}

void AccountPopup::doAppear() {
    auto overlay = ui::Layout::create();
    overlay->setContentSize(Director::getInstance()->getVisibleSize());
    overlay->setTouchEnabled(true);
    overlay->setBackGroundColor(Color3B::BLACK);
    overlay->setBackGroundColorOpacity(0xaa);
    overlay->setBackGroundColorType(BackGroundColorType::SOLID);
    this->addChild(overlay, -1);
}

void AccountPopup::doDisappear() {
    this->removeFromParent();
}

void AccountPopup::onLeft(cocos2d::Ref *sender) {
    reinterpret_cast<ui::Widget *>(sender)->setTouchEnabled(false);
	if (!_is_left_null_callback) {
		if (_left_callback) {
			_left_callback();
		}
	}
	doDisappear();
}

void AccountPopup::onRight(cocos2d::Ref *sender) {
    reinterpret_cast<ui::Widget *>(sender)->setTouchEnabled(false);
	if (!_is_right_null_callback) {
		if (_right_callback) {
			_right_callback();
		}
	}
	doDisappear();
	
}

void AccountPopup::onClose(Ref * sender) {
    reinterpret_cast<ui::Widget *>(sender)->setTouchEnabled(false);
	doDisappear();
}

ui::Widget::ccWidgetClickCallback AccountPopup::onLocateClickCallback(const std::string &name) {
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
        CLICK_MAP(AccountPopup, onLeft),
        CLICK_MAP(AccountPopup, onRight),
        CLICK_MAP(AccountPopup, onClose)
    };
    
    if (function_map.find(name) != function_map.end()) {
        return function_map.at(name);
    }
    
    return nullptr;
}
