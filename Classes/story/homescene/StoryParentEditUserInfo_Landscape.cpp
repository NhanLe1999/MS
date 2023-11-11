#include "StoryParentEditUserInfo_Landscape.h"
#include "MJHelper.h"
#include "APUpdateUserInfoAPI.h"
#include "StoryLanguageManager.h"
#include "StoryAlert.h"
#include "APDatabase.h"
#include "MSEventLogger.h"
#include "MMFirebaseDatabase.h"

USING_NS_CC;

INITIALIZE_READER(StoryParentEditUserInfo_Landscape);

std::once_flag story_parent_page_edit_user_info_reader_flag;

StoryParentEditUserInfo_Landscape * StoryParentEditUserInfo_Landscape::createView()
{

	std::call_once(story_parent_page_edit_user_info_reader_flag, [] {
		REGISTER_CSB_READER(StoryParentEditUserInfo_Landscape);
	});
//	auto csb_name = mj::helper::getCSBName("csb/StoryParentEditUserInfo_Landscape.csb");
	auto p = static_cast<StoryParentEditUserInfo_Landscape *>(cocos2d::CSLoader::createNodeWithVisibleSize("csb/StoryParentEditUserInfo_Landscape.csb"));
	if (p) {
		p->didLoadFromCSB();
		return p;
	}

	CC_SAFE_DELETE(p);
	return nullptr;
}

void StoryParentEditUserInfo_Landscape::didLoadFromCSB()
{
    onChangeDisplayLanguage(LANGUAGE_MANAGER->getDisplayLangId());
    
    utils::findChild<ui::Layout*>(this, "layout")->setTouchEnabled(false);
    
	auto user_info = ap::Database::getInstance().getUserInfo();
	box_name = utils::findChild<ui::Layout*>(this, "box_name");
	box_address = utils::findChild<ui::Layout*>(this, "box_address");
	box_email = utils::findChild<ui::Layout*>(this, "box_email");
	box_phone = utils::findChild<ui::Layout*>(this, "box_phone");

    text_name = utils::findChild<ui::TextField *>(box_name, "textfield");
	setupTextField(text_name, user_info._name);
    text_name->addEventListener(CC_CALLBACK_2(StoryParentEditUserInfo_Landscape::onTextfieldChanged, this));
    text_name->setMaxLength(15);
    
    text_address = utils::findChild<ui::TextField *>(box_address, "textfield");
	setupTextField(text_address, user_info._address);
    text_address->addEventListener(CC_CALLBACK_2(StoryParentEditUserInfo_Landscape::onTextfieldChanged, this));
    text_address->setMaxLength(15);
    
	text_mail = utils::findChild<ui::TextField *>(box_email, "textfield");
	setupTextField(text_mail, user_info._mail);
	text_mail->addEventListener(CC_CALLBACK_2(StoryParentEditUserInfo_Landscape::onTextfieldChanged, this));
	
	text_phone = utils::findChild<ui::TextField *>(box_phone, "textfield");
	setupTextField(text_phone, user_info._phone);
	text_phone->addEventListener(CC_CALLBACK_2(StoryParentEditUserInfo_Landscape::onTextfieldChanged, this));
}

void StoryParentEditUserInfo_Landscape::onEnter()
{
	ui::Layout::onEnter();
    ms::EventLogger::getInstance().logEvent("view_edit_user_info", {});
	this->setContentSize(Director::getInstance()->getWinSize());
	auto csb_name = mj::helper::getCSBName("csb/StoryParentEditUserInfo_Landscape.csb");
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("show", false);
}

void StoryParentEditUserInfo_Landscape::onSave(cocos2d::Ref * sender)
{
    Button* btn = (Button*)sender;
    ms::EventLogger::getInstance().logEvent("saved_user_info", {});
	auto txt_name = utils::findChild<ui::TextField *>(box_name, "textfield");
	auto txt_address = utils::findChild<ui::TextField *>(box_address, "textfield");
	auto txt_email = utils::findChild<ui::TextField *>(box_email, "textfield");
	auto txt_phone = utils::findChild<ui::TextField *>(box_phone, "textfield");

	auto text_name = txt_name->getString();
	auto text_mail = txt_email->getString();
	auto text_phone = txt_phone->getString();
	//TODO: validate data
	auto is_valid_name = (!text_name.empty());
	auto is_valid_mail = mj::helper::isValidEmail(text_mail);
	auto is_valid_phone = (!text_phone.empty() && text_phone.size() >=9 && text_phone.size() <= 12 );
    
	auto is_valid_address = true;//!txt_address->getString().empty();
	auto is_vn = LANGUAGE_MANAGER->getDisplayLangId() == 4;

	// hide all error

	hideError(box_name);
	hideError(box_address);
	hideError(box_email);
	hideError(box_phone);
	std::string msg;
	if (text_name.length() == 0) {
		msg = is_vn ? StringUtils::StringUTF8("Không được bỏ trống tên").getAsCharSequence() : "Name must not be blank";
		showError(box_name, msg);
	}

	if (text_phone.length() == 0)
	{
		msg = is_vn ? StringUtils::StringUTF8("Không được bỏ trống số điện thoại").getAsCharSequence() : "Phone number must not be blank";
		showError(box_phone, msg);
	}
	else if ((!is_valid_phone)) {
		msg = is_vn ? StringUtils::StringUTF8("Vui lòng nhập đúng định dạng cho số điện thoại").getAsCharSequence() : "Please enter a valid phone number";
		showError(box_phone, msg);
	}

	if (text_mail.empty())
	{
		msg = is_vn ? StringUtils::StringUTF8("Không được bỏ trống email").getAsCharSequence() : "Email must not be blank";
		showError(box_email, msg);
	}
	else if (!is_valid_mail) {
		msg = is_vn ? StringUtils::StringUTF8("Vui lòng nhập đúng định dạng cho email").getAsCharSequence() :"Please enter a valid email";
		showError(box_email, msg);
	}

	if (!is_valid_address)
	{
		msg = is_vn ? StringUtils::StringUTF8("Không được bỏ trống địa chỉ").getAsCharSequence() : "Address must not be blank";
		showError(box_address, msg);
	}

	if (!is_valid_name || !is_valid_mail || !is_valid_address || !is_valid_phone) return;
    if (btn!=NULL){
        btn->setTouchEnabled(false);
    }
	auto user_info = ap::Database::getInstance().getUserInfo();
	user_info._name = txt_name->getString();
	user_info._mail = txt_email->getString();
	user_info._phone = txt_phone->getString();
	user_info._address = txt_address->getString();

	ap::api::UpdateUserInfo::call(user_info, true, [this,btn,is_vn](bool is_success, std::string msg) {
		if (is_success)
		{
			//TODO: Thong bao
			//StoryAlert::showAlert(is_vn ? "Thông tin của bạn đã được sửa thành công." : "The user info was updated successfully", "OK", is_vn ? "Đóng" : "Close");
           ms::EventLogger::getInstance().logEvent("saved_user_info_done", {}); Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("key.event.account.profile.edit.user.info.success");
			onClose(this);
		}
		else
		{
			Director::getInstance()->getScheduler()->performFunctionInCocosThread([btn,msg, is_vn] {
                if (btn!=NULL){
                    btn->setTouchEnabled(true);
                }
                ms::EventLogger::getInstance().logEvent("saved_user_info_fail", {});
                if(msg == ""){
                    StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
                }else{
                    //TODO: show notification
                    StoryAlert::showAlert(msg, "OK", is_vn ? "Đóng" : "Close");
                }
			});
		}

	});
}

void StoryParentEditUserInfo_Landscape::onClose(cocos2d::Ref * sender)
{
    UICCTextField *ime = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(utils::findChild<Layout*>(this, "box_name"), "textfield")->getVirtualRenderer());
     ime->closeIME();
    UICCTextField *ime1 = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(utils::findChild<Layout*>(this, "box_email"), "textfield")->getVirtualRenderer());
    ime1->closeIME();
    UICCTextField *ime2 = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(utils::findChild<Layout*>(this, "box_phone"), "textfield")->getVirtualRenderer());
    ime2->closeIME();
    UICCTextField *ime3 = dynamic_cast<UICCTextField*>(utils::findChild<ui::TextField*>(utils::findChild<Layout*>(this, "box_address"), "textfield")->getVirtualRenderer());
    ime3->closeIME();

    
	auto csb_name = mj::helper::getCSBName("csb/StoryParentEditUserInfo_Landscape.csb");
	auto runani = CSLoader::createTimeline(csb_name);
	runani->setTag(1014);
	this->runAction(runani);
	runani->play("hide", false);
	runani->setLastFrameCallFunc([=] {
		this->removeFromParent();
	});
}

void StoryParentEditUserInfo_Landscape::onChangeDisplayLanguage(int lang_id){
    utils::findChild<ui::Text*>(utils::findChild<Layout*>(this, "box_phone"), "title")->setString((lang_id == 1)? "Phone number" : "Số điện thoại");
    utils::findChild<ui::Text*>(utils::findChild<Layout*>(this, "box_address"), "title")->setString((lang_id == 1)? "Address" : "Địa chỉ");
    utils::findChild<ui::Text*>(utils::findChild<Layout*>(this, "box_name"), "title")->setString((lang_id == 1)? "User name" : "Tên người dùng");
    
    utils::findChild<ui::TextField*>(utils::findChild<Layout*>(this, "box_phone"), "textfield")->setPlaceHolder((lang_id == 1)? "Your phone number" : "Nhập số điện thoại");
    utils::findChild<ui::TextField*>(utils::findChild<Layout*>(this, "box_email"), "textfield")->setPlaceHolder((lang_id == 1)? "Your email" : "Nhập email");
    utils::findChild<ui::TextField*>(utils::findChild<Layout*>(this, "box_address"), "textfield")->setPlaceHolder((lang_id == 1)? "Your address" : "Nhập địa chỉ");
    utils::findChild<ui::TextField*>(utils::findChild<Layout*>(this, "box_name"), "textfield")->setPlaceHolder((lang_id == 1)? "Your name" : "Nhập tên người dùng");
    
    utils::findChild<Button*>(this, "btn_done")->setTitleText((lang_id == 1)? "Done" : "Xong");
    
    utils::findChild<Text*>(this, "text1")->setString((lang_id == 1)? "Account information" : "Thông tin tài khoản");
}

cocos2d::ui::Widget::ccWidgetClickCallback StoryParentEditUserInfo_Landscape::onLocateClickCallback(const std::string & callBackName)
{
	std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map = {
		CLICK_MAP(StoryParentEditUserInfo_Landscape, onSave),
		CLICK_MAP(StoryParentEditUserInfo_Landscape, onClose)
	};

	if (function_map.find(callBackName) != function_map.end()) {
		return function_map.at(callBackName);
	}

	return nullptr;
}

void StoryParentEditUserInfo_Landscape::onTextfieldChanged(cocos2d::Ref * pSender, cocos2d::ui::TextField::EventType type)
{

	auto box = utils::findChild(this, "bg_ngang");


	switch (type)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	case TextField::EventType::ATTACH_WITH_IME:
		box->runAction(MoveBy::create(0.25, Vec2(0, 200)));
		break;
	case TextField::EventType::DETACH_WITH_IME:
		box->runAction(MoveBy::create(0.25, Vec2(0, -200)));
		break;
#endif
	case TextField::EventType::INSERT_TEXT:
	{
		CCLOG("INSERT_TEXT - %s", text_mail->getString().c_str());
		auto utf8 = StringUtils::StringUTF8(text_mail->getString());
		if (utf8.length() >= 1) {
			if (utf8.getString().at(utf8.length() - 1)._char == " ") {
				utf8.deleteChar(utf8.length() - 1);
				text_mail->setString(utf8.getAsCharSequence());
			}
		}

		utf8 = StringUtils::StringUTF8(text_phone->getString());
		if (utf8.length() >= 1) {
			auto new_char = utf8.getString().at(utf8.length() - 1)._char;

			if (new_char == " " || (new_char != "+" && !mj::helper::isValidNumber(new_char))) {
				utf8.deleteChar(utf8.length() - 1);
				text_phone->setString(utf8.getAsCharSequence());
			}
		}
        utf8 = StringUtils::StringUTF8(text_name->getString());
        if (utf8.length() > 15) {
            auto new_char = utf8.getString().at(utf8.length() - 1)._char;
                utf8.deleteChar(utf8.length() - 1);
                text_name->setString(utf8.getAsCharSequence());
            
        }
        utf8 = StringUtils::StringUTF8(text_address->getString());
        if (utf8.length() > 15) {
            auto new_char = utf8.getString().at(utf8.length() - 1)._char;
                utf8.deleteChar(utf8.length() - 1);
                text_address->setString(utf8.getAsCharSequence());
            
        }
        auto text = text_name->getString();
          if (text.length() >= 1) {
              text_name->setString(mj::helper::ltrim(text));
          }
             
          text = text_address->getString();
          if (text.length() >= 1) {
              text_address->setString(mj::helper::ltrim(text));
          }
        
	}
	break;
	default:
		break;
	}

}

void StoryParentEditUserInfo_Landscape::hideError(cocos2d::Node * sender)
{
	auto img_error = utils::findChild<ui::ImageView*>(sender, "img_error");
	auto text_error = utils::findChild<ui::Text*>(sender, "text_error");

	if (img_error) img_error->setVisible(false);
	if (text_error) text_error->setVisible(false);
}

void StoryParentEditUserInfo_Landscape::showError(cocos2d::Node * sender, std::string message)
{
	auto img_error = utils::findChild<ui::ImageView*>(sender, "img_error");
	auto text_error = utils::findChild<ui::Text*>(sender, "text_error");

	if (!img_error || !text_error) return;

	img_error->setVisible(true);
	text_error->setVisible(true);

	text_error->setString(message);
	img_error->setPositionX(text_error->getPositionX() - text_error->getVirtualRendererSize().width / 2 - img_error->getContentSize().width / 2);
}

void StoryParentEditUserInfo_Landscape::setupTextField(cocos2d::Ref * sender, std::string value)
{
	auto textfield = (ui::TextField*)(sender);
	textfield->setCursorEnabled(true);
	textfield->setCursorChar('|');
	textfield->setTextHorizontalAlignment(TextHAlignment::CENTER);
	textfield->setTextVerticalAlignment(TextVAlignment::CENTER);
	//text_name->setMaxLengthEnabled(true);
	//text_name->setMaxLength(25);
	textfield->setPlaceHolderColor(Color4B(0, 0, 0, 0x44));
	textfield->setString(value);
}
