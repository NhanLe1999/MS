//
//  MiniSudokuQuiz.h
//  MJQuiz
//
//  Created by HuyPhuong on 8/1/17.
//
//

#ifndef MiniSudokuQuiz_h
#define MiniSudokuQuiz_h

#include <stdio.h>
#include "Math_Quiz.h"


/*
 Note game choose correct objects
 - Đã hoàn thành:
 1. Game cơ bản.
 2. Game tô màu.
 
 - Chưa hoàn thành:
 1. So sánh với object mẫu (làm nhưng chưa test)
 2. Sắp xếp lại trước khi chọn đáp án.
 3. kiểu i spy: 1 câu hỏi mẫu cho tất cả các kind.
 4. Game so sánh khác.
 */
class MiniSudokuQuiz : public Math_Quiz
{
public:
    static MiniSudokuQuiz* createQuiz();
    
    void initQuiz(mj::model::Game game) override;
    void onStart()override;
    void startQuestion()override;
    void onExplanation(bool correct)override;
    void onComplete()override;
    void autoTesting() override;
    
private:
    bool init() override;
    
    int sudoku_size;
    cocos2d::Size item_size;
    bool display_icon;
    bool has_block = false;
    math::comparision::Key comparision_key;
    std::vector<math::resource::Image*> img_list;
    
    math::resource::Image* bg_white = nullptr;
    math::resource::Image* bg_red = nullptr;
    math::resource::Image* bg_green = nullptr;
    math::resource::Image* bg_gray = nullptr;
    
    //callback Func
    bool start_dock;
    void onMoveObjectBegan(cocos2d::Node* node);
    void onMoveObjectEnded(cocos2d::Node* node, cocos2d::Vec2 point);
    void checkSudokuDone(cocos2d::Node* node);
    
    void suggestSudoku();
    
    std::vector<Math_Object*> object_dock;
    std::vector<std::vector<int>> iblocks; //Chi số các block
    std::vector<int> m_sudoku; //Sudoku đáp án
    std::vector<int> i_sudoku; //Sudoku đang giải
    std::vector<int> m_suggest; //Danh sach vi tri suggest
    std::vector<int> list_conflic;
    
    void sudokuGenerate();
    void sudokuSuggestion(std::vector<int> sudoku);
    bool checkSudoku(std::vector<int> sudoku);
    bool checkSudokuRow(std::vector<int> sudoku);
    bool checkSudokuColumn(std::vector<int> sudoku);
    bool checkSudokuBlocks(std::vector<int> sudoku);
    bool checkSudokuAtPoint(int value, int point);
    
    void onBeganDragSudoku(int value);
    void onEndedDragSudoku(int value);
};


#endif /* MiniSudokuQuiz_h */
