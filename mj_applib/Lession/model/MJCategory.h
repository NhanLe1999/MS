//
//  MJCategory.h
//  MonkeyMath-mobile
//
//  Created by HuyPhuong on 4/2/18.
//

#ifndef MJCategory_h
#define MJCategory_h

#include <stdio.h>
#include "MJBaseModel.h"
#include "MJActivity.h"
#include "MJLesson.h"
#include "cocos2d.h"
namespace mj {
    namespace model {
        class Category : public BaseModel {
        public:
            using BaseModel::BaseModel;
//            Category(int, int, std::string);
            
            CC_SYNTHESIZE(int, _parent_id, ParentId);
            
            CC_SYNTHESIZE_READONLY(std::vector<Category>, _childrent, Childrent);
            CC_SYNTHESIZE_READONLY(std::vector<Activity>, _activities, Activities);
            CC_SYNTHESIZE_READONLY(std::vector<Lesson>, _lessons, Lessons);
            
            void addActivity(Activity activity);
            void addLesson(Lesson lesson);
            
            void addChild(Category category);
        };
    }
}
#endif /* MJCategory_h */
