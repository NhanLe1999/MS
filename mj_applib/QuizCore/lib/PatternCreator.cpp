//
//  PatternCreater.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 6/20/17.
//
//

#include "PatternCreator.h"

PatternCreator::PatternCreator(int _piece, int _size, std::string _type, bool shuffle)
{
    pattern_type=_type;
    piece=_piece;
    size=_size;
    
    while (pattern.size()<_size) {
        std::vector<int> piece_index;
        for (int i=0; i<_piece; i++) {
            piece_index.push_back(i);
        }
        if (shuffle) {
            math::func::shuffle(piece_index);
        }
        
        for (int index : piece_index) {
            if (pattern.size()<_size) {
                pattern.push_back(index);
            } else {
                break;
            }
        }
    }
    if (shuffle) {
        math::func::shuffle(pattern);
    }
}

int PatternCreator::getIndexMatchGrowing(int index) {
    int index_match = 0;
    switch (growing_type) {
        case Pattern_2_left:
        {
            int number_in_step = 2;
            int number_remain = index+1;
            while (number_remain > number_in_step) {
                number_remain -= number_in_step;
                number_in_step++;
            }
            index_match = number_remain == number_in_step ? 0 : 1;
        }
            break;
        case Pattern_2_right:
        {
            int number_in_step = 2;
            int number_remain = index+1;
            while (number_remain > number_in_step) {
                number_remain -= number_in_step;
                number_in_step++;
            }
            index_match = number_remain == number_in_step ? 1 : 0;
        }
            break;
        default:
        {
            int number_in_step = 3;
            int number_remain = index+1;
            while (number_remain > number_in_step) {
                number_remain -= number_in_step;
                number_in_step++;
            }
            if (number_remain == 1) {
                index_match = 0;
            } else if (number_remain == number_in_step) {
                index_match = 2;
            } else {
                index_match = 1;
            }
        }
            break;
    }
    return index_match;
}

void PatternCreator::setPatternListSize(int size)
{
    int pattern_size=(int)pattern.size();
    if (pattern_type == "growing") {
        for (int i=0; i<size; i++) {
            pattern_match.push_back(pattern[getIndexMatchGrowing(i)]);
        }
    } else {
        for (int i=0; i<size; i++) {
            pattern_match.push_back(pattern[i%pattern_size]);
        }
    }
}

std::vector<int> PatternCreator::checkPattern()
{
    std::vector<int> not_matchs;
    
    for (int i=0; i<pattern_match.size(); i++) {
        if (i<pattern_list.size()) {
            Math_Object* m_object=pattern_list[i];
            
            if (pattern_type.compare("icon")==0) {
                math::resource::Image* m_image=match_icon[pattern_match[i]];
                if (m_object->compare(cocos2d::Value(m_image->code), math::comparision::exactly)!=0) {
                    not_matchs.push_back(i);
                }
            } else if (pattern_type.compare("size")==0) {
                int m_size=match_size[pattern_match[i]];
                if (m_object->compare(cocos2d::Value(m_size), math::comparision::size) !=0) {
                    not_matchs.push_back(i);
                }
            } else if (pattern_type.compare("position")==0) {
                int m_size=match_position[pattern_match[i]];
                if (m_object->compare(cocos2d::Value(m_size), math::comparision::position) !=0) {
                    not_matchs.push_back(i);
                }
            } else if (pattern_type.compare("growing")==0) {
                math::resource::Image* m_image=match_icon[pattern_match[i]];
                if (m_object->compare(cocos2d::Value(m_image->code), math::comparision::exactly)!=0) {
                    not_matchs.push_back(i);
                }
            }
        } else {
            not_matchs.push_back(i);
        }
    }
    return not_matchs;
}

void PatternCreator::addPatternImage(math::resource::Image* image)
{
    if (match_icon.size()<piece) {
        match_icon.push_back(image);
    }
}

void PatternCreator::addPatternSize(int size)
{
    if (match_size.size()<piece) {
        match_size.push_back(size);
    }
}

void PatternCreator::addPatternPosition(int position)
{
    if (match_position.size()<piece) {
        match_position.push_back(position);
    }
}

math::resource::Image* PatternCreator::getImageAt(int index)
{
    if (pattern_type.compare("growing")==0) {
        int index_match = getIndexMatchGrowing(index);
        return match_icon[index_match];
    } else {
        return match_icon[pattern_match[index]];
    }
}

int PatternCreator::getSizeLevelAt(int index)
{
    return match_size[pattern_match[index]];
}


int PatternCreator::getPositionAt(int index)
{
    return match_position[pattern_match[index]];
}

void PatternCreator::setGrowingType(PatternGrowingType type) {
    if (piece == 2) {
        if (type == Pattern_3) {
            growing_type = cocos2d::random(0, 1) == 0 ? Pattern_2_left : Pattern_2_right;
        } else {
            growing_type = type;
        }
    } else {
        growing_type = Pattern_3;
    }
    
}

