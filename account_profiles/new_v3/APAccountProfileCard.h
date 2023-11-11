//
//  APAccountProfileCard.h
//  MJStory
//
//  Created by Hieu Nguyen on 8/10/20.
//

#ifndef APAccountProfileCard_h
#define APAccountProfileCard_h

#include <stdio.h>
#include "cocostudiomacro2.h"
#include "APProfile.h"

class APAccountProfileCardDelegate
{
public:
    virtual void onClickProfileCard(cocos2d::Ref * sender) {};
    virtual void onClickProfileIcon(cocos2d::Ref * sender) {};
};

class APAccountProfileCard : public cocos2d::ui::Layout,public cocostudio::WidgetCallBackHandlerProtocol
{
public:
    enum APCARD_MODE
    {
        NORMAL,
        EDITABLE
    };
    
    CREATE_FUNC(APAccountProfileCard);
    
    static APAccountProfileCard* createCard(const ap::Profile profile,APCARD_MODE card_mode = APCARD_MODE::NORMAL);
    
    APCARD_MODE getCardMode();
    void setCardMode(APCARD_MODE card_mode);
    void setDelegate(APAccountProfileCardDelegate *delegate);
    void setProfile(ap::Profile profile);
    ap::Profile getProfile();
    
private:
    void didLoadFromCSB(const ap::Profile profile, APCARD_MODE card_mode);
    
    ap::Profile                     m_profile;
    APCARD_MODE                     m_card_mode;
    cocos2d::ui::ImageView *        m_img_mode;
    cocos2d::ui::ImageView *        m_img_avatar;
    cocos2d::ui::Text *             m_lbl_name;
    cocos2d::ui::Text *             m_lbl_age;
    
    APAccountProfileCardDelegate *  _delegate;
    
    void onClickCard(cocos2d::Ref * sender);
    void onClickIcon(cocos2d::Ref * sender);
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string &name) override;
};

CREATE_READER(APAccountProfileCard);
#endif /* APAccountProfileCard_h */
