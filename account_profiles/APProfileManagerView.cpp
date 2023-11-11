#include "APProfileManagerView.h"
#include "APChooseProfileView.h"
#include "APDatabase.h"
#include "api/APUpdateUserInfoAPI.h"
#include "APAuth.h"
#include "MSIntro.h"
#include "MJHelper.h"
#include "StoryAlert.h"
#include "LicenseInfoModel.h"
#include "StoryConfigManager.h"
#include "MMFirebaseDatabase.h"

USING_NS_CC;

INITIALIZE_READER(APProfileManagerView);

std::once_flag ap_profile_manager_view_reader_flag;

APProfileManagerView * APProfileManagerView::createView()
{
	std::call_once(ap_profile_manager_view_reader_flag, []
	{
		REGISTER_CSB_READER(APProfileManagerView);
	});
	auto node = reinterpret_cast<APProfileManagerView *>(CSLoader::createNodeWithVisibleSize("csb/account/AccountManager.csb"));
	if (node)
	{
		node->didLoadFromCSB();
		return node;
	}

	CC_SAFE_DELETE(node);
	return nullptr;

}

void APProfileManagerView::didLoadFromCSB()
{
    if(CONFIG_MANAGER->isVietnameseStories()){
        utils::findChild<Layout*>(this, "layout_root")->setBackGroundImage("mjstory/new_ui_vn/bg.jpg");
    }
	//TODO: update hạn sử dụng

	// Load Thông tin tài khoản

	auto user_info = ap::Database::getInstance().getUserInfo();
	txt_name = utils::findChild<ui::TextField*>(this, "txt_name");
	txt_email = utils::findChild<ui::TextField*>(this, "txt_email");
	txt_phone = utils::findChild<ui::TextField*>(this, "txt_phone");
	txt_address = utils::findChild<ui::TextField*>(this, "txt_address");

	txt_name->setCursorEnabled(true);
	txt_name->setCursorChar('|');
	txt_name->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	txt_email->setCursorEnabled(true);
	txt_email->setCursorChar('|');
	txt_email->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	txt_phone->setCursorEnabled(true);
	txt_phone->setCursorChar('|');
	txt_phone->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	txt_address->setCursorEnabled(true);
	txt_address->setCursorChar('|');
	txt_address->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));

	txt_name->setString(user_info._name);
	txt_email->setString(user_info._mail);
	txt_phone->setString(user_info._phone);
	txt_address->setString(user_info._address);
}

void APProfileManagerView::onEnter()
{
	ui::Layout::onEnter();
}

void APProfileManagerView::onExit()
{
	ui::Layout::onExit();
}

void APProfileManagerView::onClickAddLicense(cocos2d::Ref * sender)
{
}

void APProfileManagerView::onClickEditProfile(cocos2d::Ref * sender)
{
	
	auto text_name = txt_name->getString();
	auto text_mail = txt_email->getString();
	auto text_phone = txt_phone->getString();
	//TODO: validate data
	auto is_valid_name = (!text_name.empty());
	auto is_valid_mail = mj::helper::isValidEmail(text_mail);
	auto is_valid_phone = (!text_phone.empty()) && (mj::helper::isValidPhoneNumber(text_phone));

	auto is_vn = LICENSE_INFO_MODEL->isDisplayLanguageVN();
	if (text_name.length() == 0) {
		StoryAlert::showAlert(is_vn ? "Vui lòng nhập tên của bạn" : "Please enter your name", "OK", is_vn ? "Đóng" : "Close");
		return;
	}

	if (text_phone.length() == 0 || (!mj::helper::isValidPhoneNumber(text_phone))) {
		StoryAlert::showAlert(is_vn ? "Vui lòng nhập đúng định dạng cho số điện thoại" : "Please enter a valid phone number", "OK", is_vn ? "Đóng" : "Close");
		return;
	}

	if (!is_valid_mail) {
		StoryAlert::showAlert(is_vn ? "Vui lòng nhập đúng định dạng cho email" : "Please enter a valid email", "OK", is_vn ? "Đóng" : "Close");
		return;
	}

	auto user_info = ap::Database::getInstance().getUserInfo();
	user_info._name = txt_name->getString();
	user_info._mail = txt_email->getString();
	user_info._phone = txt_phone->getString();
	user_info._address = txt_address->getString();

	ap::api::UpdateUserInfo::call(user_info, true ,[=](bool is_success, std::string msg) {
		if (is_success)
		{
			//TODO: Thong bao

		}
		else 
		{
			//TODO: Thong bao
		}

	});
}

void APProfileManagerView::onClickDetailProfile(cocos2d::Ref * sender)
{
	auto profile_choose_view = APChooseProfileView::createView();
	Director::getInstance()->getRunningScene()->addChild(profile_choose_view, INT_MAX - 1);
}

void APProfileManagerView::onClickSignOut(cocos2d::Ref * sender)
{
	ap::Auth::signOut();
	//reload data
	//auto scene = MSIntro::createScene();
	//Director::getInstance()->replaceScene(TransitionMoveInR::create(0.5f, scene));

}

void APProfileManagerView::onClose(cocos2d::Ref * sender)
{
	this->removeFromParent();
}

cocos2d::ui::Widget::ccWidgetClickCallback APProfileManagerView::onLocateClickCallback(const std::string & name)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(APProfileManagerView, onClickAddLicense),
		CLICK_MAP(APProfileManagerView, onClickEditProfile),
		CLICK_MAP(APProfileManagerView, onClickDetailProfile),
		CLICK_MAP(APProfileManagerView, onClose)
	};

	if (function_map.find(name) != function_map.end()) {
		return function_map.at(name);
	}

	return nullptr;
}
