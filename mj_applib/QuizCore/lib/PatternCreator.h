//
//  PatternCreater.h
//  MJQuiz
//
//  Created by HuyPhuong on 6/20/17.
//
//

#ifndef PatternCreater_h
#define PatternCreater_h

#include <stdio.h>
#include "Math_Libs.h"
#include "Math_Object.h"
#include "Math_Func.h"

class PatternCreator {
public:
    PatternCreator(int _piece, int _size, std::string _type, bool shuffle = true);
    ~PatternCreator(){}
    enum PatternGrowingType {
        Pattern_2_right = 0,
        Pattern_2_left,
        Pattern_3
    };
    
    std::vector<int> pattern;
    //thể hiện của pattern code bằng hình ảnh.
    std::vector<math::resource::Image*> match_icon;
    std::vector<int> match_size;
    std::vector<int> match_position;
    std::vector<Math_Object*> pattern_list;
    std::vector<int> pattern_match;
    
    void setGrowingType(PatternGrowingType type);
    int getIndexMatchGrowing(int index);

    void setPatternListSize(int size);
    std::vector<int> checkPattern();
    
    void addPatternImage(math::resource::Image* image);
    void addPatternSize(int size);
    void addPatternPosition(int position);

    math::resource::Image* getImageAt(int index);
    int getSizeLevelAt(int index);
    int getPositionAt(int index);
    
    std::string pattern_type;
    int piece;
    int size;
    
    PatternGrowingType growing_type = PatternGrowingType::Pattern_2_right;
};
#endif /* PatternCreater_h */
