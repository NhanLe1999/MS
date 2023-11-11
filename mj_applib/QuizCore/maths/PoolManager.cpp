//
//  PoolManager.cpp
//  MJQuiz
//
//  Created by HuyPhuong on 7/10/17.
//
//

#include "PoolManager.h"
#include "CocoStudioMacro.h"

USING_NS_CC;
using namespace cocos2d;
using namespace std;

static Math_Pool* _instance=NULL;
Math_Pool* Math_Pool::getInstance()
{
    if (!_instance) {
        _instance=new Math_Pool();
        
        REGISTER_READER(Math_ObjectReader);
        REGISTER_READER(Math_SlotReader);
        REGISTER_READER(Math_AnswerReader);
        REGISTER_READER(Math_QuestionReader);
        REGISTER_READER(Math_GroupReader);
    }
    return _instance;
}

/**
 Clear all object in all quizzes of lesson
 */
void Math_Pool::resetAll() {
    resetSlots();
    resetGroups();
    resetObjects();
    resetAnswers();
    resetQuestions();
    resetImages();
}

/**
 Clear all images
 */
void Math_Pool::resetImages()
{
    for (math::resource::Image* img : pooled_images) {
        img->clear();
    }
    pooled_images.clear();
}

/**
 Clear all Math_Object
 */
void Math_Pool::resetObjects()
{
    for (Math_Object* node : pooled_objects) {
        node->setDisable();
    }
    pooled_objects.clear();
}

/**
 Clear all Math_Slot
 */
void Math_Pool::resetSlots()
{
    for (Math_Slot* node : pooled_slots) {
        node->setDisable();
    }
    pooled_slots.clear();
}

/**
 Clear all Math_Answer
 */
void Math_Pool::resetAnswers()
{
    for (Math_Answer* node : pooled_answers) {
        node->setDisable();
    }
    pooled_answers.clear();
}

/**
 Clear all Math_Question
 */
void Math_Pool::resetQuestions()
{
    for (Math_Question* node : pooled_questions) {
        node->setDisable();
    }
    pooled_questions.clear();
}

/**
 Clear all Math_Group
 */
void Math_Pool::resetGroups()
{
    for (Math_Group* node : pooled_groups) {
        node->setDisable();
    }
    pooled_groups.clear();
}

/**
 Create new math Image

 @return new Image
 */
math::resource::Image* Math_Pool::getImage()
{
    math::resource::Image* img=new math::resource::Image ();
    pooled_images.push_back(img);
    return img;
}

/**
 Create new math object

 @return new Math_Object
 */
Math_Object* Math_Pool::getObject()
{
    Math_Object* node=Math_Object::createObject();
    node->retain();
    pooled_objects.push_back(node);
    return node;
}

/**
 Ceate new math slot

 @return new Math_slot
 */
Math_Slot* Math_Pool::getSlot()
{
    Math_Slot* node=Math_Slot::createSlot();
    node->retain();
    pooled_slots.push_back(node);
    return node;
}

/**
 Create new math answer

 @return new Math_Answer
 */
Math_Answer* Math_Pool::getAnswer()
{
    Math_Answer* node=Math_Answer::createAnswer();
    node->retain();
    pooled_answers.push_back(node);
    return node;
}

/**
 Create new math question

 @return new Math_Question
 */
Math_Question* Math_Pool::getQuestion()
{
    Math_Question* node=Math_Question::createQuestion();
    node->retain();
    pooled_questions.push_back(node);
    return node;
}

/**
 Create new math group

 @return new Math_Group
 */
Math_Group* Math_Pool::getGroup()
{
    Math_Group* node=Math_Group::createGroup();
    node->retain();
    pooled_groups.push_back(node);
    return node;
}
