//
//  PoolManager.h
//  MJQuiz
//
//  Created by HuyPhuong on 7/10/17.
//
//

#ifndef PoolManager_h
#define PoolManager_h

#include <stdio.h>

#include "Math_Libs.h"
#include "Math_Object.h"
#include "Math_Slot.h"
#include "Math_Answer.h"
#include "Math_Question.h"
#include "Math_Group.h"

#include <map>

#define mpool Math_Pool::getInstance()

class Math_Pool
{
public:
    Math_Pool(){}
    static Math_Pool* getInstance();
    
    math::resource::Image* getImage();
    Math_Object* getObject();
    Math_Slot* getSlot();
    Math_Answer* getAnswer();
    Math_Question* getQuestion();
    Math_Group* getGroup();
    
    void resetAll();
    
private:
    void resetImages();
    void resetObjects();
    void resetSlots();
    void resetGroups();
    void resetAnswers();
    void resetQuestions();

    std::vector<math::resource::Image*> pooled_images;
    std::vector<Math_Object*> pooled_objects;
    std::vector<Math_Slot*> pooled_slots;
    std::vector<Math_Answer*> pooled_answers;
    std::vector<Math_Question*> pooled_questions;
    std::vector<Math_Group*> pooled_groups;

};

#endif /* PoolManager_h */
