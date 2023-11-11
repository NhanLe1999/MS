//
//  StoryParentNotification_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/13/17.
//
//

#ifndef StoryParentNotification_Landscape_hpp
#define StoryParentNotification_Landscape_hpp

#include "StoryParentSubPage_Landscape.h"
#include "StoryParentNotificationItem_Landscape.h"
#include "MJRequest.h"

#include "story/popup/ViralPopup/ViralPopup_Item.h"

class StoryParentNotification_Landscape : public StoryParentSubPage_Landscape, public mj::RequestDelegate {
public:
    CREATE_FUNC(StoryParentNotification_Landscape);
    static StoryParentNotification_Landscape * createPage();
	void onNotificationItemClicked(StoryNotificationInfo i_info);
protected:
	virtual ~StoryParentNotification_Landscape();
	void didLoadFromCSB() override;

    void onRequestSuccess(int tag, rapidjson::Value data) override;
    void onRequestFailed(int tag, int error, std::string message, rapidjson::Value data) override;

    void loadNotification();

    mj::Request * _request = nullptr;
    std::vector<StoryNotificationInfo> _notifications_info;
    void addMessageFreeReadStory();
private:
	std::string m_messageTitle = "none";
	void pushCleverTapEvents();

	void getDataInAppInbox();
};

CREATE_READER_CLASS(StoryParentNotification_LandscapeReader, StoryParentNotification_Landscape);

#endif /* StoryParentNotification_Landscape_hpp */
