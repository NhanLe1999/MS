//
//  MJGame.cpp
//  M
//
//  Created by To Ngoc Linh on 4/27/17.
//
//

#include "MJGame.h"

namespace mj {
    namespace model {
        Game::Game(int id, std::string name, std::string schema) :
            _json_schema(schema) {
                _id = id;
                _name = name;
        }
    }
}
