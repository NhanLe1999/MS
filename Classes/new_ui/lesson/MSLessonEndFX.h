//
//  MSLessonEndFX.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 12/26/17.
//

#ifndef MSLessonEndFX_hpp
#define MSLessonEndFX_hpp

#include "cocostudiomacro2.h"
#include "MSPreActivity.h"
#include "GAFWrapper.h"
class MSLessonEndFX : public cocos2d::ui::Layout, public ms::delegate::PreActivity {
public:
    CREATE_FUNC(MSLessonEndFX);
    static MSLessonEndFX * createFX(int lesson_id);
    
private:
    void onEnter() override;
    
    int _lesson_id;
    
    void onStartOver() override;
    void onBackToLesson() override;
    void onContinueLearn(mj::model::ActInfo) override;
    void showEndingLesson();
    void showFireWork();
    GAFWrapper *phao_giay = nullptr, /**phao_trai, *phao_phai, *phao_hoa,*/ *monkey_dance= nullptr;

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
    GAFWrapper *phao_trai = nullptr, *phao_phai = nullptr, *phao_hoa = nullptr;
#endif
};

CREATE_READER(MSLessonEndFX);

#endif /* MSLessonEndFX_hpp */
