//
//  MazesHelper.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 4/24/17.
//
//

#include "MazeCreator.h"
#include "Math_Libs.h"
#include "Math_Func.h"


USING_NS_CC;
using namespace std;


MazeCreator::MazeCreator(int colNumber, int rowNumber, int start, int end, int path, bool fit_size)
{
	number_column = colNumber;
	number_row = rowNumber;
    start_col = start >= number_column ? (number_column - 1) : start;
    end_col = end >= number_column ? (number_column - 1) : end;
    maze_size = path < (number_row + abs(start_col - end_col)) ? (number_row + abs(start_col - end_col)) : path;
    
    if (path == 0) {
        maze_size = 0;
        doSearchPath(start_col, end_col);
    } else {
        if (maze_size % 2 == 0) {
            if ((start_col+end_col+number_row) % 2 == 1) {
                if (fit_size) {
                    if (end_col == number_column-1) {
                        end_col--;
                    } else {
                        end_col++;
                    }
                } else {
                    maze_size += (cocos2d::random(0, 1)*2-1);
                }
            }
        } else {
            if ((start_col+end_col+number_row) % 2 == 0) {
                if (fit_size) {
                    if (end_col == number_column-1) {
                        end_col--;
                    } else {
                        end_col++;
                    }
                } else {
                    maze_size += (cocos2d::random(0, 1)*2-1);
                }
            }
        }

        while (result.size() != maze_size) {
            doSearchPath(start_col, end_col);
        }
    }
    
    std::string log = "";
    for (int i1=0; i1<number_column * number_row; i1++) {
        int value = 0;
        for (int i2=0; i2<result.size(); i2++) {
            if (result[i2] == i1) {
                value = 1;
                break;
            }
        }
        log = cocos2d::StringUtils::format("%s%d  ", log.c_str(), value);
        if (i1 % number_column == number_column-1) {
            CCLOG("%s", log.c_str());
            log = "";
        }
    }
    CCLOG("Maze create done");
}

bool MazeCreator::isSame(MazeCreator *other) {
    if (other == nullptr) {
        return false;
    }
    if (number_row != other->number_row) {
        return false;
    }
    if (number_column != other->number_column) {
        return false;
    }
    if (result.size() != other->result.size()) {
        return false;
    }
    for (int i=0; i<result.size(); i++) {
        if (result[i]!=other->result[i]) {
            return false;
        }
    }
    return true;
}

void MazeCreator::doSearchPath(int begin, int end) {
    for (int i = 0; i<number_column*number_row; i++) {
        flag[i] = 0;
    }
    result.clear();
    is_searching = true;
    flag[begin] = 1;
    Try(begin);
}

//Dem so diem lien ke kha dung
int MazeCreator::countNext(int cur)
{
	int number_next = 0;
	int cur_x = cur % number_column;
	int cur_y = cur / number_column;

	next[0] = 0;
	next[1] = 0;
	next[2] = 0;
	next[3] = 0;

	next_check[0] = 0;
	next_check[1] = 0;
	next_check[2] = 0;
	next_check[3] = 0;

    //Next left
	if (cur_x>0 && flag[cur - 1] == 0) {
		number_next++;
		next_check[0] = 1;
		next[0] = cur - 1;
	}

	//Next right
	if (cur_x<number_column - 1 && flag[cur + 1] == 0) {
		number_next++;
		next_check[1] = 1;
		next[1] = cur + 1;
	}

	//Next up
	if (cur_y>0 && flag[cur - number_column] == 0) {
		number_next++;
		next_check[2] = 1;
		next[2] = cur - number_column;
	}

	//Next down
	if (cur_y<number_row - 1 && flag[cur + number_column] == 0) {
		number_next++;
		next_check[3] = 1;
		next[3] = cur + number_column;
	}
	return number_next;
}

/** Tru 2 diem dau va cuoi. cac diem khac deu co 2 diem lien ke
*/
bool MazeCreator::isValid(int cur)
{
	if (cur == 0 ||
		cur == number_column * number_row - 1 ||
		cur == number_column - 1 ||
		cur == number_column * (number_row - 1)) {
		//4 dinh
		if (countNext(cur)<1) {
			return false;
		}
	}
	else if ((cur > 0 && cur < number_column - 1) ||
		(cur > number_column * number_row - number_column && cur < number_column * number_row - 1) ||
		(cur % number_column == 0) ||
		(cur % number_column == number_column - 1)) {
		//4canh
		if (countNext(cur)<2) {
			return false;
		}
	}
	else {
		if (countNext(cur)<3) {
			return false;
		}
	}
	return true;
}

/*Thuat toan nhanh can*/
void MazeCreator::Try(int i) {
	if (!is_searching) {
		return;
	}
	flag[i] = 1; //Da duyet
	if (isValid(i)) {
		result.push_back(i);
        
        if (maze_size != 0) {
            int remain_path = maze_size - (int)result.size();
            if (remain_path < minPathFrom(i, number_column * (number_row - 1) + end_col)) {
                result.erase(result.begin() + result.size() - 1);
                flag[i] = 0; //Bo da duyet
                return;
            }
        }
        
		if (i == number_column * (number_row - 1) + end_col) {
			is_searching = false;
		} else {
			//array check
			std::vector<int> checks;
			for (int j = 0; j<4; j++) {
				if (next_check[j] == 1) {
					checks.push_back(next[j]);
				}
			}
            math::func::shuffle(checks);
            
			for (int j = 0; j<checks.size(); j++) {
				Try(checks[j]);
			}
			if (!is_searching) {
				return;
			}
			result.erase(result.begin() + result.size() - 1);
			flag[i] = 0;
		}
	}
	else {
		flag[i] = 0; //Bo da duyet
	}
}

int MazeCreator::minPathFrom(int i1, int i2) {
    if (i1 == i2) {
        return 0;
    }
    int col_i1 = i1 % number_column;
    int row_i1 = i1 / number_column;
    
    int col_i2 = i2 % number_column;
    int row_i2 = i2 / number_column;
    return abs(col_i1 - col_i2) + abs(row_i1 - row_i2);
}

std::vector<int> MazeCreator::checkPoint(int point) {
    std::vector<int> rets;
    
    //Check left
    if ((point % number_column) > 0) {
        for (int index = 0; index < result.size(); index++) {
            if (result[index] == point-1) {
                rets.push_back(index+1);
            }
        }
    }
    
    //Check right
    if ((point % number_column) < number_column - 1) {
        for (int index = 0; index < result.size(); index++) {
            if (result[index] == point + 1) {
                rets.push_back(index+1);
            }
        }
    }
    
    //Check up
    if (point >= number_column) {
        for (int index = 0; index < result.size(); index++) {
            if (result[index] == point - number_column) {
                rets.push_back(index+1);
            }
        }
    }
    
    //Check down
    if (point < number_column * (number_row-1)) {
        for (int index = 0; index < result.size(); index++) {
            if (result[index] == point + number_column) {
                rets.push_back(index+1);
            }
        }
    }
    
    return rets;
}

