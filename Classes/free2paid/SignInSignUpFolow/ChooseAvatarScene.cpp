#include "ChooseAvatarScene.h"


USING_NS_CC;
INITIALIZE_READER(ChooseAvatarScene);

#define MS_CHOOSE_AVATAR_SCENE_NAME "ChooseAvatarScene"

std::once_flag ms_f2p_choose_avatar_reader;//4.

ChooseAvatarScene* ChooseAvatarScene::createView()
{
    std::call_once(ms_f2p_choose_avatar_reader, []
        {
            REGISTER_CSB_READER(ChooseAvatarScene);
        });

    auto view = static_cast<ChooseAvatarScene*>(CSLoader::createNodeWithVisibleSize("csb/free2paid/SignInSignUpFlow/ChooseAvartar.csb"));

    if (view)
    {
        view->didLoadFromCSB();

        return view;
    }

    CC_SAFE_DELETE(view);

    return nullptr;
}

void ChooseAvatarScene::didLoadFromCSB()
{
	flag = true;

	std::vector<std::string> list_avatar = {
		"boy_1.png", "boy_2.png", "boy_3.png", "boy_4.png", "boy_5.png", "boy_6.png", "boy_7.png", "boy_8.png",
		"girl_1.png", "girl_2.png", "girl_3.png", "girl_4.png", "girl_5.png", "girl_6.png", "girl_7.png"
	};
	mj::helper::shuffle(list_avatar);
	_listview = utils::findChild<ui::ListView*>(this, "listview");
	_listview->setMagneticType(ui::ListView::MagneticType::CENTER);
	_listview->setScrollBarEnabled(false);

	for (const auto& p : list_avatar) {
		auto wrapper = ui::Layout::create();
		wrapper->setContentSize(Size(275, 300));
		wrapper->setBackGroundColorType(ui::Layout::BackGroundColorType::NONE);
		wrapper->setBackGroundColor(Color3B(random(0, 0xff), random(0, 0xff), random(0, 0xff)));
		auto found = p.find("mjstory/avatar");
		auto avatar_path = found != std::string::npos ? p.substr(found) : p;
		wrapper->setName(avatar_path);
		auto ava = ui::ImageView::create(p);
		ava->setContentSize(Size(300, 300));
		ava->setAnchorPoint(Point::ANCHOR_MIDDLE);
		ava->setPosition(Vec2(275.0f / 2, 150));
		ava->setName("avatar");
		wrapper->addChild(ava);
		ava->ignoreContentAdaptWithSize(false);

		_listview->pushBackCustomItem(wrapper);
	}

	_listview->forceDoLayout();
	_listview->jumpToLeft();
	_listview->scrollToItem(7, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE, 0.5f);

    LANGUAGE_MANAGER->changeDisplayLanguage(this);//6.0
}

void ChooseAvatarScene::returnPreviousScreen(cocos2d::Ref* sender)
{
	auto timePicker = BirthdayPicker::createView([=](int day, int month, int year) {
		});
	Director::getInstance()->getRunningScene()->addChild(timePicker, INT_MAX - 1);

	timePicker->scrollToDefaultData(1, 1, 2019);

    this->removeFromParent();
}

cocos2d::ui::Widget::ccWidgetClickCallback ChooseAvatarScene::onLocateClickCallback(const std::string& callBackName)
{
    std::unordered_map<std::string, ui::Widget::ccWidgetClickCallback> function_map =
    {
        CLICK_MAP(ChooseAvatarScene, returnPreviousScreen),
		CLICK_MAP(ChooseAvatarScene, onNext)
    };

    if (function_map.find(callBackName) != function_map.end())
    {
        return function_map.at(callBackName);
    }

    return nullptr;
}

void ChooseAvatarScene::onCallUpdateProfileAPI(ap::Profile profile){
    
}
void ChooseAvatarScene::CallAPIProfileList()
{
    this->runAction(Sequence::create(DelayTime::create(0.5f),
        CallFunc::create([=] {
            Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
                std::string userId = std::to_string(ap::Database::getInstance().getUserInfo()._id);
                ap::api::ProfilesList::callV2(userId, CC_CALLBACK_2(ChooseAvatarScene::APIProfileListCallBack, this));
                
            });
            }),
        NULL));
  
}

void ChooseAvatarScene::APIProfileListCallBack(bool isSucess, std::string msg)
{
    this->runAction(Sequence::create(DelayTime::create(0.2f),
        CallFunc::create([=] {
        if (isSucess)
        {
        }
        else
        {
            // show thong bao loi
        }
            
    }),NULL));



}
void ChooseAvatarScene::onNext(Ref* sender)
{
	auto button = (ui::Button*)sender;
	button->setTouchEnabled(false);

	// get current avatar
	this->unscheduleUpdate();
	_listview->stopOverallScroll();
	if (auto current_avatar = _listview->getCenterItemInCurrentView()) {
		_avatar = current_avatar->getName();
	}
	_acountName = MJDEFAULT->getStringForKey(key_f2p_acount_name);
	ap::Profile profile;

    if (int profileID =  MJDEFAULT->getIntegerForKey(key_f2p_skip_profile_id,0))
	{
		profile = ap::Database::getInstance().getProfileById(profileID);
	}
	if (profile.profile_id <= 0)
		profile.profile_id = 0;
	profile.name = _acountName;
	profile.avatar = "mjstory/avatar/" + _avatar;
	_birthOfDate = MJDEFAULT->getArrayIntegerForKey(key_f2p_birth_day, {});
	if (_birthOfDate.size() >= 3)
	{
		profile.day_of_birth = _birthOfDate[0];
		profile.month_of_birth = _birthOfDate[1];
		profile.year_of_birth = _birthOfDate[2];
	}
	ap::Database::getInstance().updateProfile(profile);
	mj::helper::showWaitView();
    //CallAPIProfileList();
	ap::api::UpdateProfile::callV2(profile, [=](bool is_success, std::string msg)
		{
			cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([=] {
				mj::helper::deleteWaitView();
				if (is_success)
				{
					CRASH_AUTO_LOG;
                    CleverTapManager::GetInstance()->pushProfileByID(PROFILE_MANAGER.getCurrentProfileId(),CleverTapManager::PUSH_TYPE::LOGIN, nullptr);
					_listview->setTouchEnabled(false);
						mj::helper::deleteWaitView();
						ap::api::LoadUpdate::call([=](bool isSuccess, std::string msg) {
							cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, isSuccess, msg] {
								if (isSuccess)
								{

								}
								else
								{
									CCLOG("call failed");
								}

								int typeUser = APPURCHASE_MANAGER->getTypeUser();
								MJDEFAULT->deleteValueForKey("on_register_success_f2p");
								if (APPURCHASE_MANAGER->isFreeUser() && !APPURCHASE_MANAGER->isSkipUser())
								{
									MJDEFAULT->deleteValueForKey(key_f2p_save_list_of_learned_items);
									MJDEFAULT->deleteValueForKey(key_f2p_save_last_date_of_learned_items);
									MJDEFAULT->deleteValueForKey(key_f2p_show_trial_alert);
									MJDEFAULT->setIntegerForKey(key_free_content_page_option, (int)ms::f2p::FreeContentOption::TEN_ITEM_FREE);
									auto scene = MSFreeContentPage::createScene();
									Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, scene));
								}
								else
								{
									gotoMsHome();
								}
								});
							});
						return;
				}
				else
				{
					if (button)
					{
						button->setTouchEnabled(true);
						button->setBright(true);
					}
					bool is_vn = StoryLanguageManager::getDisplayLangId() == 4;
					//TODO: show notification and do something
					if (msg == "") {
						StoryAlert::showAlert(StringUtils::format("%s", LANGUAGE_MANAGER->getTextByKey(MJDEFAULT->getIntegerForKey(key_lang_display, 1), "parent.common.error").c_str()), "OK", "");
					}
					else {
						//TODO: show notification
						StoryAlert::showAlert(msg, "OK", is_vn ? "Đóng" : "Close");
					}
				}
				button->setTouchEnabled(true);
				}
			);
		});
}

void ChooseAvatarScene::onEnter() {
	ui::Layout::onEnter();
	this->schedule(schedule_selector(ChooseAvatarScene::updateFX));
}

void ChooseAvatarScene::updateFX(float dt)
{
	auto items = _listview->getItems();
	const auto middle_point = Point(Director::getInstance()->getVisibleSize() / 2);
	const auto radius = 350.0f;
	const auto middle_radius = 125.0f;
	for (auto i : items) {
		auto position = i->getPosition() + i->getContentSize() / 2;
		position = i->getParent()->convertToWorldSpace(position);
		position = position - middle_point;
		auto ratio = 0.5f;
		if (fabs(position.x) < middle_radius)
		{
			ratio = sqrt(middle_radius * middle_radius - position.x * position.x);
			ratio = 0.5f + 0.5f * ratio / middle_radius;
		}
		i->getChildByName("avatar")->setScale(ratio);
		i->getChildByName("avatar")->setOpacity(ratio * 0xff);
	}
}

void ChooseAvatarScene::gotoMsHome()
{
	CRASH_AUTO_LOG;
	CCLOG("MSFreeContentPage::gotoMsHome");

	auto home = MSHome::createScene();
	if (!CONFIG_MANAGER->isAppActive())
	{
		if (MJDEFAULT->getBoolForKey(key_onboarding_part_2, false))
		{
			if (MSInstallPromotion::shouldShowPopup())
			{
				auto popup = MSInstallPromotion::createView(true);
				home->addChild(popup);
			}
			else
			{
				if (!MJDEFAULT->getBoolForKey("is_user_skip", false) && !MJDEFAULT->getBoolForKey("key_parent_in_choose_view_button_clicked", false))
				{
					if (APPURCHASE_MANAGER->getTypeUser() == IAP::user::TYPE_USERS::USER_EXPRIED)
					{
					}
				}
			}
		}
	}
	if (MJDEFAULT->getBoolForKey("key_parent_in_choose_view_button_clicked", false))
	{
		MJDEFAULT->setBoolForKey("key_parent_in_choose_view_button_clicked", false);
		if (auto homeView = (MSHome*)home->getChildByName(MSHOME_NAME))
		{
			auto waitView = APWaitView::createView(std::string(""), false, true);
			waitView->setName("waiting_view");
			home->addChild(waitView, INT_MAX - 1);
			homeView->runAction(Sequence::createWithTwoActions(DelayTime::create(3.0f), CallFunc::create([=]() {
				MJDEFAULT->setBoolForKey(key_show_parent_lock, false);
				waitView->removeFromParent();
				homeView->clearLayouts();
				homeView->goToParent();
				})));
		}
	}
	// Update phần setting ngôn ngữ ở đây
	StoryLanguageManager::getInstance()->changeMapLangDisplay(MJDEFAULT->getIntegerForKey(key_lang_display, 1));
	Director::getInstance()->replaceScene(TransitionSlideInR::create(0.5, home));
}
