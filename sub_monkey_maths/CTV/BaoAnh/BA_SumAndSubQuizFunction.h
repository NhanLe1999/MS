//
//  BA_SumAndSubQuizFunction.h
//  MJQuiz
//
//  Created by AnhB Nguyen on 9/7/17.
//
//

#ifndef BA_SumAndSubQuizFunction_h
#define BA_SumAndSubQuizFunction_h

#include "BA_SumAndSubQuiz.hpp"


//--------------------------------------------------
//BA gen function
void BA_SumAndSubQuiz::genOperator(std::string oprt){
    
    std::string oprtTemp;
    int obj_id =0;
    std::vector<math::resource::Image*> oprt_img = getOperators();
    
    oprt_size.width = drager_size.width - 50;
    oprt_size.height = drager_size.height - 50;
    
    //random oprt, random expression;
    if(!oprt.compare("none")){
        oprtTemp = oprt_List[ rand() % oprt_List.size() ];
    }
    else{
        oprtTemp = oprt;
    }
    
    if(!oprtTemp.compare("+")){
        auto plusImg = oprt_img.at(0);
       // auto plusImg = math::resource::ResHelper::getImage (cocos2d::StringUtils::format("%soperators.json", _game_path.c_str()), 0);
        Math_Object* plus = Math_Pool::getInstance()->getObject();
        plus->setEnable(obj_id,math::common::deny, plusImg, NULL );
        plus->setObjectSize(oprt_size);
        plus->setTag(1);
        plus->setImageVisible(false);
        plus->setValueText("+");
        plus->showValueTextWithAnimation(false);
        plus->getLabelValue()->setTextColor(cocos2d::Color4B(236,28,36,255));
        plus->getLabelValue()->setFontSize(object_size.width*2);
        math_symbols.push_back(plus);
        
        
    }
    else if (!oprtTemp.compare("-")){
         auto minusImg = oprt_img.at(1);
       // auto minusImg = math::resource::ResHelper::getImage(cocos2d::StringUtils::format("%soperators.json", _game_path.c_str()), 1);
        Math_Object* minus = Math_Pool::getInstance()->getObject();
        minus->setEnable(obj_id,math::common::deny, minusImg, NULL );
        minus->setObjectSize(oprt_size);
        minus->setTag(-1);
        minus->setImageVisible(false);
        minus->setValueText("-");
        minus->showValueTextWithAnimation(false);
        minus->getLabelValue()->setTextColor(cocos2d::Color4B(236,28,36,255));
        minus->getLabelValue()->setFontSize(object_size.width*2);
        math_symbols.push_back(minus);
        
        
    }
    else{
        auto equalImg = oprt_img.at(2);
        //auto equalImg = math::resource::ResHelper::getImage(cocos2d::StringUtils::format("%soperators.json", _game_path.c_str()), 2);
        Math_Object* equal = Math_Pool::getInstance()->getObject();
        equal->setEnable(obj_id,math::common::deny, equalImg, NULL );
        equal->setObjectSize(oprt_size);
        equal->setTag(0);
        equal->setImageVisible(false);
        equal->setValueText("=");
        equal->showValueTextWithAnimation(false);
        equal->getLabelValue()->setTextColor(cocos2d::Color4B(236,28,36,255));
        equal->getLabelValue()->setFontSize(object_size.width*2);
        math_symbols.push_back(equal);
        
    }
    
}

void BA_SumAndSubQuiz::genObject(math::resource::Image* obj_img){
    Math_Object* object=Math_Pool::getInstance()->getObject();
    object->setEnable(obj_id++, math::common::touch, obj_img, NULL);
    object->setObjectSize(drager_size);
    object->setSelected(false);
    
    object->setTouchCallback(CC_CALLBACK_1(BA_SumAndSubQuiz::onTouchedObject, this));
    
    _objects.push_back(object);
}

void BA_SumAndSubQuiz::genGroup(int i, int random_img ,math::group::GroupType gr_type, math::resource::Image* group_bg, std::vector<math::resource::Image*> ob_list){
    
    
    Math_Group* group=Math_Pool::getInstance()->getGroup();
    _group_list.clear();
    
    //
    _group_list = createGroupList(obj_id++, number_objects[i], ob_list, random_img);
    
    if(game_mode == 1){
        drag_values.push_back(number_objects[i]);
        group_column= randomColRow(number_objects[i]).first;
        group_row = randomColRow(number_objects[i]).second;
        
    }
    
    group->setEnable(i, _group_list, gr_type, group_cap);
    
    group->setObjectSize(object_size);
    group->setComparisionKey(sort_key);
    
    if (gr_type==math::group::frame) {
        group->setGroupFrames(group_column, group_row, group_bg);
        group->setAlignObjects(math::group::left);
    }
    else if (gr_type==math::group::queue){
        
        group->setGroupQueue(group_size);
        group->setBackground(group_bg);
    }
    //set up group with custom config
    else {
        group->setGroupSize(group_size);
        group->setBackground(group_bg);
    }
    group->setGroupSize(group_size);
    _groups.push_back(group);
}

void BA_SumAndSubQuiz::genSlot(math::resource::Image* slot_bg){
    
    Math_Object* object=Math_Pool::getInstance()->getObject();
    object->setEnable(obj_id++, math::common::deny, slot_bg, NULL);
    
    Math_Slot* slot=Math_Pool::getInstance()->getSlot();
    slot->setEndble(obj_id++, slot_bg);
    slot->setSlotSize(slot_size);
    slot->setComparisionValue(object->getComparisionValue());
    _slots.push_back(slot);
    _slot_object.push_back(slot);
    
    _slot_object.push_back(object);
    
}

void BA_SumAndSubQuiz::genDrager(math::resource::Image* drag_img){
    
    auto drag_bg = math::resource::ResHelper::createImage(getUIPath("common/drag.png"));
    Math_Object* object=Math_Pool::getInstance()->getObject();
    object->setEnable(obj_id++, math::common::deny, drag_img, drag_bg);
    object->setObjectSize(slot_size);
    
    object->setMoveCallback(nullptr, CC_CALLBACK_2(BA_SumAndSubQuiz::onMoveObjectEnded, this));
    object->setValueTextByKey("number");
    object->setImageVisible(false);
    object->showValueTextWithAnimation(false);
    object->getLabelValue()->setTextColor(cocos2d::Color4B(236,28,36,255));
    //object->getLabelValue()->setAnchorPoint(cocos2d::Vec2(0.5, 0.65));
    object->getLabelValue()->setFontSize(object_size.width*1.5);
    _dragers.push_back(object);
}

void BA_SumAndSubQuiz::genQuestion(){
    
}


//--------------------------------------------------
//BA function
std::vector<Math_Object*> BA_SumAndSubQuiz::createGroupList(int obj_id, int obj, std::vector<math::resource::Image*> obList, int random){
    
    std::vector<Math_Object*> groupList;
    
    for(int i = 0; i < obj; i++){
        
        Math_Object* object = Math_Object::createObject();
        object->setEnable(obj_id++, math::common::deny , obList[random], NULL);
        object->setObjectSize(object_size);
        object->onShow(0);
        groupList.push_back(object);
    }
    
    
    return groupList;
};


//--------------------------------------------------
//BA funtion
std::pair<int, int> BA_SumAndSubQuiz::randomColRow(int number){
    
    std::pair<int, int> dimensions;
    
    double number_obj_sqrt = sqrt(number);
    
    if(number <= 0){
        group_column = 1;
        group_row = 1;
    }
    else{
        
        if(number_obj_sqrt > (int)number_obj_sqrt+0.5){
            group_column = group_row = (int)number_obj_sqrt +1;
            
            
        }else if(number_obj_sqrt == (int)number_obj_sqrt){
            group_column = group_row = (int)number_obj_sqrt;
        }else{
            group_column = (int)number_obj_sqrt +1;
            group_row = (int)number_obj_sqrt;
        }
    }
    dimensions.first = group_column;
    dimensions.second = group_row;
    
    
    return dimensions;
}


//--------------------------------------------------
//BA function
void BA_SumAndSubQuiz::createSum(bool repeatNo){
    
    number_objects.clear();
    
    int sum;
    
    int index_1 = cocos2d::random(0, (int)value_list.size()-1);
    int index_2 = index_1;
    
    while(index_1 == index_2){
        index_2 = cocos2d::random(0, (int)value_list.size()-1);
    }
    
    
    if(!oprt_json.compare("-")){
        if(index_1 > index_2){
            sum = value_list.at(index_1) - value_list.at(index_2);
            number_objects.push_back(value_list.at(index_1));
            number_objects.push_back(value_list.at(index_2));
        }else{
            sum = value_list.at(index_2) - value_list.at(index_1);
            number_objects.push_back(value_list.at(index_2));
            number_objects.push_back(value_list.at(index_1));
        }
       
    }else{
        sum = value_list.at(index_1) + value_list.at(index_2);
        number_objects.push_back(value_list.at(index_1));
        number_objects.push_back(value_list.at(index_2));
    }

    
//    for (int i = 0; i < number_group; i++){
//        number_object = value_list[i];
//
//        CCLOG("number object: %i", number_object);
//        number_objects.push_back(number_object);
//        sum += number_object;
//
//    }
    
    
    if(checkSum(sum) == false){
        createSum(false);
    } else {
        
        number_objects.push_back(sum);
    }

}

//--------------------------------------------------
//BA function
void BA_SumAndSubQuiz::createObjectAndSum(){
    
    int temp = 0;
    number_objects.clear();
    
    for(int i = 0; i < number_group; i++){
        
        int number_object ;
        
        number_object = cocos2d::random(number_min, number_max);
        
        number_objects.push_back(number_object);
        
        if(!oprt_json.compare("none")){
            
            if(i ==0){
                temp = number_object;
            }else{
                
                if(math_symbols[i-1]->getTag() == 1){
                    temp += number_object;
                }else if(math_symbols[i-1]->getTag() == -1){
                    temp -= number_object;
                }else{
                    
                }
            }
            
        }else if (!oprt_json.compare("=")){
            
        }else {
            temp+= number_object;
        }
        
    }
    number_objects.push_back(temp);
    
    
    //----------------------------
    //sua lai tham so
    if(!checkSum(number_objects[number_objects.size()-1])){
        createObjectAndSum();
    }
    
    //----------------------------
    if(math_symbols[0]->getTag() == -1){
        if(number_objects[0] < number_objects[1]){
            std::swap(number_objects[1], number_objects[2]);
            std::swap(math_symbols[0], math_symbols[1]);
        }
    }
}

//--------------------------------------------------
//BA function
bool BA_SumAndSubQuiz::checkSum(int check){
    //int check = number_objects[number_objects.size()-1];
    
    if(check < min_range || check > max_range ){
        return false;
    }
    return true;
}

#endif /* BA_SumAndSubQuizFunction_h */
