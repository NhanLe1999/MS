//
//  MJBaseModel.cpp
//  M
//
//  Created by To Ngoc Linh on 5/3/17.
//
//

#include "MJBaseModel.h"

mj::model::BaseModel::BaseModel() : _id(-1), _order(-1), _name("") {
}

mj::model::BaseModel::BaseModel(int id, std::string name, int order) :
    _id(id),
    _name(name),
    _order(order) {
}
