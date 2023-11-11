//
//  ActionFlipPage.hpp
//  Azu-mobile
//
//  Created by To Ngoc Linh on 8/15/17.
//

#ifndef ActionFlipPage_hpp
#define ActionFlipPage_hpp

#include "cocos2d.h"

class ActionFlipPage : public cocos2d::ActionInterval {
public:
    static ActionFlipPage * create(float duration, bool is_reverse);
    virtual ActionFlipPage * clone() const override;
    virtual ActionFlipPage * reverse() const override;
    virtual void startWithTarget(cocos2d::Node * target) override;
    virtual void update(float time) override;

private:
    ActionFlipPage() {}
    virtual ~ActionFlipPage() {}
    bool initWithDuration(float duration, bool is_reverse);

private:
    cocos2d::Mat4 _original_transform;
    bool _is_reverse;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(ActionFlipPage);
};

#endif /* ActionFlipPage_hpp */
