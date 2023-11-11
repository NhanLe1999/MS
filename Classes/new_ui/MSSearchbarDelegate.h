//
//  MSSearchbarDelegate.h
//  MJStory
//
//  Created by To Ngoc Linh on 11/9/17.
//

#ifndef MSSearchbarDelegate_h
#define MSSearchbarDelegate_h

namespace ms {
    namespace delegate {
        class Searchbar {
        public:
            virtual void onSelectGrade() = 0;
            virtual void onDeselectGrade(int grade_id) = 0;
            virtual void onSelectSearch() = 0;
        };
    }
}

#endif /* MSSearchbarDelegate_h */
