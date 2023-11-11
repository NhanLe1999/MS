//
//  MJActivity.cpp
//  MonkeyMath-mobile
//
//  Created by HuyPhuong on 4/2/18.
//

#include "MJActivity.h"

namespace mj {
    namespace model {
        Activity::Activity(int id, int order, std::string name, std::string link) :
        _link(link) {
            _id = id;
            _order = order;
            _name = name;
        }
    }
}
