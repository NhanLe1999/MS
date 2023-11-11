//
//  MJGame.h
//  M
//
//  Created by To Ngoc Linh on 4/27/17.
//
//

#ifndef MJGame_h
#define MJGame_h

#include <stdio.h>
#include "MJBaseModel.h"
#include "cocos2d.h"
namespace mj {
    namespace model {
        class Game : public BaseModel {
        public:
			Game() {};
			~Game() {}; 
			using BaseModel::BaseModel;
            Game(int, std::string, std::string);

            CC_SYNTHESIZE(std::string, _class_name, ClassName);
            CC_SYNTHESIZE(std::string, _json_schema, JsonSchemaPath);
        };
    }
}

#endif /* MJGame_h */
