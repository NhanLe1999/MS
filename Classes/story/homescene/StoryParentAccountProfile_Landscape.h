#ifndef StoryParentAccountProfile_Landscape_hpp
#define StoryParentAccountProfile_Landscape_hpp

#include "cocostudiomacro2.h"
#include "StoryParentSubPage_Landscape.h"

class StoryParentAccountProfile_Landscape : public StoryParentSubPage_Landscape
{
public:
	CREATE_FUNC(StoryParentAccountProfile_Landscape);
	static StoryParentAccountProfile_Landscape * createPage();
	void onEditProfile();
private:
	~StoryParentAccountProfile_Landscape();
	void didLoadFromCSB();
	void onEnter() override;
	void onExit() override;
    void onChangeDisplayLanguage(int lang_id);
	
	void loadUserInfo();
	void loadLicenseInfo();
	void loadProfileInfo();

	void onClickAddLicense(cocos2d::Ref * sender);
	void onClickEditProfile(cocos2d::Ref * sender);
	void onClickDetailProfile(cocos2d::Ref * sender);
	void onClickSignOut(cocos2d::Ref * sender);
	void onClose(cocos2d::Ref * sender);
	cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & name) override;

	cocos2d::ui::TextField * txt_name;
	cocos2d::ui::TextField * txt_email;
	cocos2d::ui::TextField * txt_phone;
	cocos2d::ui::TextField * txt_address;

	bool edit_profile = false,
		edit_user_info = false,
		detail_profile = false,
		log_out = false,
		upgrade_account = false;//?
};

CREATE_READER(StoryParentAccountProfile_Landscape);
#endif // !StoryParentAccountProfile_Landscape_hpp
