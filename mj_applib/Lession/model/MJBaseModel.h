//
//  MJBaseModel.h
//  M
//
//  Created by To Ngoc Linh on 5/3/17.
//
//

#ifndef MJBaseModel_h
#define MJBaseModel_h

#include <stdio.h>
#include "cocos2d.h"
#define CC_SYNTHESIZE_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType& get##funName(void) { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

namespace mj {
    namespace model {
        class BaseModel {
        public:
            explicit BaseModel();
            explicit BaseModel(int, std::string, int);

            CC_SYNTHESIZE_READONLY(int, _id, Id);
            CC_SYNTHESIZE_READONLY(int, _order, Order);
            CC_SYNTHESIZE_READONLY(std::string, _name, Name);
        };
    }
}

#endif /* MJBaseModel_h */
