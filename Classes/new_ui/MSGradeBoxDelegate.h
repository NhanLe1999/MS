//
//  MSGradeBoxDelegate.hpp
//  MJStory
//
//  Created by To Ngoc Linh on 11/9/17.
//

#ifndef MSGradeBoxDelegate_hpp
#define MSGradeBoxDelegate_hpp

namespace ms {
    namespace delegate {
        class GradeBox {
        public:
            virtual bool isGradeBoxFromMaterialScreen() {return false;} // mặc định trả về false nhé
            virtual void onSelectGradeBox() = 0;
            virtual void onDeselectGradeBox(int grade_id) = 0;
        };
    }
}

#endif /* MSGradeBoxDelegate_hpp */
