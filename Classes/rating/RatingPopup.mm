#include "RatingPopup.h"
#include "StoryConfigManager.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)||(CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

#import <StoreKit/StoreKit.h>

void RatingPopup::doRate()
{

    //method 01:
    //int appID = CONFIG_MANAGER->isVietnameseStories() ?1388216940:1236664909;
    
    //CCLOG("appid = %d",appID);
    
    //static NSString *const iOS7AppStoreURLFormat = @"itms-apps://itunes.apple.com/app/id%d";
    //static NSString *const iOSAppStoreURLFormat = @"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=%d";
  
    //[NSURL URLWithString:[NSString stringWithFormat:([[UIDevice currentDevice].systemVersion floatValue] >= 7.0f)? iOS7AppStoreURLFormat: iOSAppStoreURLFormat, appID]];
    
    //method 02:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    [SKStoreReviewController requestReview];
#else
    if (@available(macOS 10.14, *))
    {
        [SKStoreReviewController requestReview];
    }
    else
    {
        if (CONFIG_MANAGER->isVietnameseStories())
        {
               Application::getInstance()->openURL("macappstore://itunes.apple.com/app/id1389646429?mt=12");
        }
        else
        {
            Application::getInstance()->openURL("macappstore://itunes.apple.com/app/id1282232882?mt=12");
        }
    }
#endif

}

#endif
