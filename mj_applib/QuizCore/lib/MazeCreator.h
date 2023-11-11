//
//  MazesHelper.h
//  MJQuiz
//
//  Created by HuyPhuong on 4/24/17.
//
//

#ifndef MazesHelper_h
#define MazesHelper_h

#include <stdio.h>
#include "cocos2d.h"
#define MAX_MAZE 10;

class MazeCreator
{
public:
	MazeCreator(int colNumber, int rowNumber, int start, int end, int path = 0, bool fit_size = true);
	std::vector <int> result;
	int flag[100];
    
    void doSearchPath(int begin, int end);

    int start_col;
    int end_col;
    int number_column;
    int number_row;
    int maze_size;
    std::vector<int> checkPoint(int point);
    
    bool isSame(MazeCreator* other);

private:
	bool is_searching = true;

	int next[4];
	int next_check[4];

	void Try(int i);
	int countNext(int cur);
	bool isValid(int cur);
    
    int minPathFrom(int i1, int i2);
    
};

#endif /* MazesHelper_h */
