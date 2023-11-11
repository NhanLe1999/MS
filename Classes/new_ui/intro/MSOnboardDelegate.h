//
//  MSOnboardDelegate.hpp
//  VN Stories
//
//  Created by AnhB Nguyen on 3/9/19.
//

#ifndef MSOnboardDelegate_hpp
#define MSOnboardDelegate_hpp

#include <stdio.h>
namespace ms {
    class MSOnboardDelegate {
    public:
        virtual void onFinishOnboard() = 0;
    };
}
#endif /* MSOnboardDelegate_hpp */
