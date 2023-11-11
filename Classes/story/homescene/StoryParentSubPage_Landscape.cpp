//
//  StoryParentSubPage_Landscape.cpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/13/17.
//
//

#include "StoryParentSubPage_Landscape.h"
#include "StoryParentLicense_Landscape.h"
#include "StoryParentCoupon_Landscape.h"
#include "StoryParentSupport_Landscape.h"
#include "StoryParentNotification_Landscape.h"
#include "StoryParentNotificationItem_Landscape.h"
#include "StoryParentLanguages_Landscape.h"
#include "StoryParentReadingLevel_Landscape.h"
#include "StoryParentDeleteCache_Landscape.h"
#include "StoryParentMusicSettings_Landscape.h"
#include "StoryParentAccountProfile_Landscape.h"

void StoryParentSubPage_Landscape::initReader() {

    REGISTER_READER(StoryParentDeleteCache_LandscapeReader);
    REGISTER_READER(StoryParentNotification_LandscapeReader);
    REGISTER_READER(StoryParentCoupon_LandscapeReader);
    REGISTER_READER(StoryParentLicense_LandscapeReader);
    REGISTER_READER(StoryParentSupport_LandscapeReader);
    REGISTER_READER(StoryParentNotificationItem_LandscapeReader);
    REGISTER_READER(StoryParentLanguages_LandscapeReader);
    REGISTER_READER(StoryParentReadingLevel_LandscapeReader);
    REGISTER_READER(StoryParentMusicSettings_LandscapeReader);
	REGISTER_READER(StoryParentAccountProfile_LandscapeReader);
}

StoryParentSubPage_Landscape * StoryParentSubPage_Landscape::createPage(StoryParentOptionTag tag)
{
    switch (tag) {
        case SPO_SUPPORT:
            return StoryParentSupport_Landscape::createPage();

        case SPO_LICENSE:
            return StoryParentLicense_Landscape::createPage();

        case SPO_TERMS_OF_USE:
            return nullptr;

        case DELETE_ACCOUNT:
            return nullptr;

        case SPO_PRIVACY_POLICY:
            return nullptr;

        case SPO_LANGUAGES:
            return StoryParentLanguages_Landscape::createPage();

        case SPO_NOTIFICATION:
            return StoryParentNotification_Landscape::createPage();

        case SPO_COUPON:
            return StoryParentCoupon_Landscape::createPage();

        case SPO_READINGLEVEL:
            return StoryParentReadingLevel_Landscape::createPage();

        case SPO_DELETE_CACHE:
            return StoryParentDeleteCache_Landscape::createPage();
            
        case SPO_MUSIC_SETTING:
            return StoryParentMusicSettings_Landscape::createPage();
		case SPO_ACCOUNT_PROFILE:
			return StoryParentAccountProfile_Landscape::createPage();
    }
    return nullptr;
}

void StoryParentSubPage_Landscape::didLoadFromCSB() {
    this->setPosition(cocos2d::Point::ZERO);
    this->setAnchorPoint(cocos2d::Point::ANCHOR_BOTTOM_LEFT);
}

void StoryParentSubPage_Landscape::adjustWidth(float width) {
    this->setContentSize(cocos2d::Size(width, this->getContentSize().height));
    cocos2d::ui::Helper::doLayout(this);
}
