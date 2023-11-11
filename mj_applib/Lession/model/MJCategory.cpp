//
//  MJCategory.cpp
//  MonkeyMath-mobile
//
//  Created by HuyPhuong on 4/2/18.
//

#include "MJCategory.h"
namespace mj {
    namespace model {
//        Category::Category(int id, int order, std::string name) {
//            _id = id;
//            _order = order;
//            _name = name;
//            _parent_id = 0;
//        }
        
        void Category::addActivity(mj::model::Activity activity) {
            _activities.push_back(activity);
            std::sort(_activities.begin(), _activities.end(), [=](const Activity & a, const Activity & b)->bool{
                return a.getOrder() < b.getOrder();
            });
        }
        
        void Category::addLesson(mj::model::Lesson lesson) {
            _lessons.push_back(lesson);
            std::sort(_lessons.begin(), _lessons.end(), [=](const Lesson & a, const Lesson & b)->bool{
                return a.getOrder() < b.getOrder();
            });
        }
        
        void Category::addChild(mj::model::Category category) {
            _childrent.push_back(category);
            std::sort(_childrent.begin(), _childrent.end(), [=](const Category & a, const Category & b)->bool{
                return a.getOrder() < b.getOrder();
            });
        }
    }
}
