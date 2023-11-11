//
//  StoryParentCoupon_Landscape.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 6/13/17.
//
//

#ifndef StoryParentCoupon_Landscape_hpp
#define StoryParentCoupon_Landscape_hpp

#include "StoryParentSubPage_Landscape.h"
#include "MJPay.h"

class StoryParentCoupon_Landscape : public StoryParentSubPage_Landscape, public mj::ValidateCouponDelegate {
public:
    CREATE_FUNC(StoryParentCoupon_Landscape);
    static StoryParentCoupon_Landscape * createPage();
    void onShow();
private:
    cocos2d::ui::Widget::ccWidgetClickCallback onLocateClickCallback(const std::string & callback_name) override;
    cocos2d::ui::Widget::ccWidgetEventCallback onLocateEventCallback(const std::string &callback_name) override;
    void onBack(Ref *sender);
    void onOK(Ref * sender);
    void changeDisplayLanguage();
    bool isBack = false;

    void startLoading();
    void endLoading();

protected:
    void didLoadFromCSB() override;
    void onEnter() override;

    cocos2d::ui::TextField *textfield_license;
    ImageView *background;
    void onValidateCouponSuccess(const std::string & coupon, const float & discount_percent) override;
    void onValidateCouponFailed(const std::string & coupon, const std::string & error) override;
    void onTextfieldChanged(cocos2d::Ref *pSender, int event);
    Vec2 pos_bottom, pos_top;
};

CREATE_READER_CLASS(StoryParentCoupon_LandscapeReader, StoryParentCoupon_Landscape);

#endif /* StoryParentCoupon_Landscape_hpp */
