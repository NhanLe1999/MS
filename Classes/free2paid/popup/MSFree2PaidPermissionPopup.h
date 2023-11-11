//
//  MSFree2PaidPermissionPopup.hpp
//  MJStory
//
//  Created by Cuong Cao Nguyen
//

#ifndef MSFree2PaidPermissionPopup_hpp
#define MSFree2PaidPermissionPopup_hpp

#include "cocostudiomacro2.h"
#include "APProfileDelegate.h"
#include "MSAISpeakingPermissionPopup.h"
#include <unordered_set>
class MSFree2PaidPermissionPopup : public MSAISpeakingPermissionPopup, public ms::delegate::AISpeakingPermissionPopupDelegate
{
    CREATE_FUNC(MSFree2PaidPermissionPopup);
public:
    
    MSFree2PaidPermissionPopup* show();
    
    void onChangeView() override;
    
    void requestPermissionNotifications();
    
    bool checkPermissionNotifications();

private:

    void onClose(cocos2d::Ref* sender) override;

    void onAccept(cocos2d::Ref* sender) override;

    MSFree2PaidPermissionPopup* mSFree2PaidPermissionPopup = nullptr;
};

#endif /* MSFree2PaidPermissionPopup_hpp */
